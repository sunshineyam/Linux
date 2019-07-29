#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <cstdio>
#include <unordered_map>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <json/json.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "base/http.h"
#include "speech.h"
#include <stdlib.h>
#define ASR_PATH "temp_file/asr.wav"
#define TTS_PATH "temp_file/tts.mp3"
#define CMD_ETC "command.etc"
#define LOG "log/log.txt"
//工具类
class Util{
    private:
       static pthread_t id;
    public:
        //File* popen(const char* command, const char* type) 执行任何shell命令,创建一个进程来打开一个管道（文件）
        //执行命令
        static bool Exec(std::string command, bool Is_Print)
        {
            FILE* fp = popen(command.c_str(), "r");
            if(fp == nullptr)
            {
                std::cerr << "popen error" << std::endl;
                return false;
            }
            if(Is_Print == true)
            {
                char c;
                while(fread(&c, 1, 1, fp)>0)
                {
                    fwrite(&c, 1, 1, stdout);
                }
            }
            pclose(fp);
            return true;
        }
        static void *ThreadRun(void *arg)
        {
            pthread_detach(pthread_self());
            const char* tips = (char*)arg;
            int i = 0;
            char bar[103] = {0};
            const char* lable = "|/-\\";
            for(;i <= 50; i++)
            {
                printf("%s[%-51s][%d%%][%c]\r", tips, bar, i*2, lable[i%4]);
                fflush(stdout);
                bar[i] = '=';
                bar[i+1] = '>';
                usleep(49000*2);
            }
            printf("\n");
            return (void*)1;
        }
        static void PrintStart(std::string tips)
        {
            pthread_create(&id, NULL, ThreadRun, (void*)tips.c_str());
        }
        static void PrintEnd()
        {
            pthread_cancel(id);
        }
};
pthread_t Util::id;
//图灵机器人交互
class Robot{
    private:
        //用户信息
        std::string api_key;
        std::string user_id;
        //用户发起请求
        aip::HttpClient client;
        std::string url;
    private:
        bool IsCodeLegal(int code)
        {
            bool result = true;
            switch(code){
                case 5000:
                    result = false;
                    break;
                case 6000:
                    result = false;
                    break;
                case 10004:
                    result = true;
                    break;
                default:
                    result = false;
                    break;
            }
            return result;
        }
        std::string MessageToJson(std::string& message)
        {
            //序列化，再填充请求参数的格式
            Json::Value root;
            Json::StreamWriterBuilder wb;
            std::ostringstream os;

            root["reqType"] = 0;

            Json::Value item_;
            Json::Value item;
            item_["text"] = message;
            item["inputText"] = item_;
            root["perception"] = item;

            item.clear();
            item["apiKey"] = api_key;
            item["userId"] = user_id;
            root["userInfo"] = item;
            std::unique_ptr<Json::StreamWriter> jsonwriter(wb.newStreamWriter());
            jsonwriter->write(root, &os);
            std::string json_str = os.str();
            //std::cout << "debug " << json_str << std::endl;
            return json_str;
        }
        std::string RequestTL(std::string& request_json)
        {
            std::string response;
            //speech.h查看client，http.h查看post方法
            //把转好的json串放在response中
           int staus_code = client.post(url, nullptr, request_json, nullptr, &response);
           if(staus_code != CURLcode::CURLE_OK)
           {
               std::cerr << "post error" << std::endl;
               return "";
           }
           return response;
        }
        //反序列化
        std::string JsonTOEchoMessage(std::string& s)
        {
            JSONCPP_STRING errs;
            Json::Value root; 
            Json::CharReaderBuilder rb;
            std::unique_ptr<Json::CharReader> const cr(rb.newCharReader());
            //把反序列化的结果放在root中，利用返回值判断是否反序列化成功
            bool res = cr->parse(s.data(), s.data()+s.size(), &root, &errs);
            if(!res || !errs.empty()){
                std::cerr << "parse error: " << errs << std::endl;
                return "";
            }
            //反序列化成功,输出结果
            int code = root["intent"]["code"].asInt();
            if(!IsCodeLegal(code))
            {
                std::cerr << "response_code error" << std::endl;
                return "";
            }
            Json::Value item = root["results"][0];
            std::string msg =  item["values"]["text"].asString();
            return msg;
        }
    public:
        Robot()
        {

            this->url = "http://openapi.tuling123.com/openapi/api/v2";
            this->api_key = "d6ce00c26207443dac9e91f7cb0d58fa";
            this->user_id = "1";
        }
        //机器人与人对话
        std::string Talk(std::string message)
        {
            //1、message转成JSON串，发给图灵
            std::string json_value = MessageToJson(message);
            //2、请求图灵 std::out
            std::string response = RequestTL(json_value);
            //3、图灵将json转message，响应给用户
            std::string echo_message = JsonTOEchoMessage(response);
            return echo_message;
        }
        ~Robot()
        {
        }
};
//创建百度语音识别类
class SpeechRec{
    private:
        std::string app_id;
        std::string api_key;
        std::string secret_key;
        aip::Speech *client;
    private:
        //判断语音识别是否成功
        bool IsLegalCode(int code)
        {
            bool result = false;
            switch(code)
            {
                case 0:
                    result = true;
                    break;
                default:
                    result = false;
                    break;
            }
            return result;
        }
    public:
        SpeechRec()
        {
            app_id = "16874266";
            api_key = "9L0xlC5EUPMIOMr40mCQKnkS";
            secret_key = "M1RP5rqkRiitR9yvzix9fbQXFoqlYlBY";
            client = new aip::Speech(app_id, api_key, secret_key);
        }
        //语音识别
        bool ASR(std::string path, std::string& out)
        {
            std::map<std::string,std::string> options;
            std::string file_content;
            aip::get_file_content(ASR_PATH, &file_content);
            options["dev_pid"] = "1536";
            Json::Value result = client->recognize(file_content, "wav", 16000, options);
            int code = result["err_no"].asInt();
            if(!IsLegalCode(code))
            {
                std::cerr << "Recongnize error" << std::endl;
                return false;
            }
            //提取出语音信息,以字符串的方式打印
            out = result["result"][0].asString();
            return true;
        }
        //语音合成
        bool TTS(std::string path, std::string message)
        {
            bool ret;
            std::ofstream ofile;
            std::string file_ret;
            std::map<std::string, std::string> options;
            options["spd"] = "9";
            options["per"] = "0";
            //将合成成功的二进制数据写入文件中
            ofile.open(TTS_PATH, std::ios::out | std::ios::binary);
            Json::Value result = client->text2audio(message, options, file_ret);
            if(!file_ret.empty())
            {
                ofile << file_ret;
                ofile.close();
                ret = true;
            }
            else
            {
                std::cerr << result.toStyledString() << std::endl;
                ret = false;
            }
            return ret;
        }
        ~SpeechRec()
        {

        }
        
};
//核心逻辑
class Jarvis{
    private:
        Robot rb;
        SpeechRec sr;
        //命令集映射
        std::unordered_map<std::string, std::string> commands;
    private:
        //录音
        bool Record()
        {
            //录音命令
            std::string command = "arecord -t wav -c 1 -r 16000 -d 5 -f S16_LE ";
            command += ASR_PATH;
            //不显示输出结果
            command += " >/dev/null 2>&1";
            std::cout << "Record done..." << std::endl;
            //不显示命令的录音
            return Util::Exec(command, false);
        }
        //播放
        bool Play()
        {
            std::string command = "cvlc --play-and-exit ";
            command += TTS_PATH;
            command += " >/dev/null 2>&1";
            //调工具函数，执行播放命令
            bool ret = Util::Exec(command, false);
            return ret;
        }
    public:
        Jarvis()
        {
        }
        //加载配置文件,，获取命令插入map中
        bool LoadEtc()
        {
            char buf[256];
            std::ifstream in(CMD_ETC);
            if(!in.is_open())
            {
                std::cerr << "open file error" << std::endl;
                return false;
            }
            std::string sep = ":";
            //按行查找
            while(in.getline(buf, sizeof(buf)))
            {
                std::string str = buf;
                size_t pos = str.find(sep);
                if(std::string::npos == pos)
                {
                    std::cerr << "Load error" << std::endl;
                    continue;
                }
                std::string k = str.substr(0,pos);
                std::string v = str.substr(pos+sep.size());
                k+="。";
                commands.insert(std::make_pair(k,v));
            }
            std::cout << "Load Commands ... success" << std::endl;
            in.close();
            return true;
        }
        //判断当期输入信息是不是命令
        bool IsCommands(std::string message, std::string& cmd)
        {
            auto it = commands.find(message);
            if(it != commands.end())
            {
                cmd = it->second;
                std::cout << cmd << std::endl;
                return true;
            }
            return false;
        }
        void Run()
        {
#ifdef _LOG_
            int fd = open(LOG, O_WRONLY|O_CREAT, 0644);
            if(fd < 0)
            {
                return;
            }
            dup2(fd,2);
#endif
            fflush(stdout);
            volatile bool quit = false;
            while(!quit)
            {
                std::cout << "Record ... " << std::endl;
                Util::PrintStart("录音中 ");
                if(this->Record())
                {
                    //语音识别
                    std::string message;
                    if(sr.ASR(ASR_PATH, message))
                    {
                        //1、判断是不是命令
                        std::string cmd = "";
                        if(IsCommands(message,cmd))
                        {
                            std::cout << "[zlj@localhost Jarvis]" << cmd << std::endl;
                            Util::Exec(cmd, true);
                            //继续下一次
                            continue;
                        }
                        //2、交给图图
                        std::cout << "波妞# " << message << std::endl;
                        if(message == "明天见。")
                        {
                            std::string quit_message = "goodbye";
                            std::cout << "波妞# " << quit_message << std::endl;
                            exit(0);
                        }
                        std::string echo = rb.Talk(message);
                        if(sr.TTS(TTS_PATH, echo))
                        {
                            //合成成功，播放
                            this->Play();
                        }
                        std::cout << "宗介# " << echo << std::endl;
                    }
                    else
                    {
                        std::cerr << "ASR  error" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Record error" << std::endl;
                }
                Util::PrintEnd();
            }
        }
        ~Jarvis()
        {

        }
};
