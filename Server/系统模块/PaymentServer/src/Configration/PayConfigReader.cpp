#include "stdafx.h"
#include "PayConfigReader.h"


using namespace boost::filesystem;
using namespace payment;

bool PayConfigReader::_bReadConfig = false;
PayConfig PayConfigReader::_config;

void PayConfigReader::readConfig(const std::string& path)
{
    try{

        std::string workPath = boost::filesystem::initial_path<boost::filesystem::path>().string();
        bool bFound = false;

        if (exists(path)){
            bFound = true;
        } else  if (exists(workPath+ "/" +path)) {
            bFound = true;
        }

        if (bFound){

            if (is_regular_file(path)){
                namespace po = boost::program_options;

                //声明需要的选项
                po::options_description desc("Allowed options");
                desc.add_options()
                    ("platform_id", po::value<std::string>(&_config.strPlatformID), "platform_id")
                    ("server_addr", po::value<std::string>(&_config.strSocektAddr), "server_addr")
                    ("server_port_socket", po::value<std::string>(&_config.strSocketPort), "server_port_socket")
                    ("server_port_http", po::value<std::string>(&_config.strHttpPort), "server_port_http")
                    ("server_http_root", po::value<std::string>(&_config.strHttpRoot), "server_http_root")
                    ("data_base_addr", po::value<std::string>(&_config.strDBAddr), "data_base_addr")
                    ("data_base_name", po::value<std::string>(&_config.strDBName), "data_base_name")
                    ("data_base_port", po::value<std::string>(&_config.strDBPort), "data_base_port")
                    ("data_base_user_name", po::value<std::string>(&_config.strDBUserName), "data_base_user_name")
                    ("data_base_password", po::value<std::string>(&_config.strDBPassWord), "data_base_password")
					("notify_url", po::value<std::string>(&_config.strNotifyUrl), "notify_url")
                    ("use_weixin_pay", po::value<bool>(&_config.bUseWeixinPay), "use_weixin_pay")
                    
                    ("AppID", po::value<std::string>(&_config.strAppID), "AppID")
                    ("MchID", po::value<std::string>(&_config.strMchID), "MchID")
                    ("Secret", po::value<std::string>(&_config.strSecretKey), "Secret")

                    ("weixin_order", po::value<std::string>(&_config.strWeiXinOrder), "weixin_order")
                    ("weixin_order_xml", po::value<std::string>(&_config.strWeiXinOrderXml), "weixin_order_xml")
                    ("products_file", po::value<std::string>(&_config.strProductFile), "products_file")
                    
                    ;

                std::ifstream settings_file(path);
                po::variables_map vm;
                po::store(po::parse_config_file(settings_file, desc), vm);
                po::notify(vm);

                if (_config.strHttpRoot == "") {
                    _config.strHttpRoot = workPath;
                }

                _bReadConfig = true;
            }else if (is_directory(path)){
                std::cout << path << " is a directory.";
            } else{
                std::cout << path << " exists, but is not a regular file or directory\n";
            }

        } else {

            std::cout << path << " does not exist\n";

        }

    } catch (const filesystem_error& ex){

        std::cout << ex.what() << '\n';

    }
}

const PayConfig& PayConfigReader::getConfig()
{
    if(_bReadConfig) {
        return _config;
    }
    throw new std::exception("Using configration before it been loaded.");
}
