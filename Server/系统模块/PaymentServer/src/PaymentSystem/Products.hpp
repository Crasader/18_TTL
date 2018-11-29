#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include "stdafx.h"

#include "json/json.h"
#include "Tools/Util.hpp"


namespace payment {

union ProductValue
{
    int nValue;
    long lValue;
    double dValue;
    char cStr[128] = {0};
};

struct ProductKeys
{
    std::string key;
    ProductValue val;

	ProductKeys& operator = (const ProductKeys& other) {
		this->key = other.key;
		this->val = other.val;
		return *this;
	}

	//bool operator == (const std::string& keyName) const {
	//	return strcmp(keyName.c_str(), key.c_str()) == 0;
	//}

	bool operator == (const char* keyName) const {
		return strcmp(keyName, key.c_str()) == 0;
	}
};

struct Product : public std::vector<ProductKeys>
{

    bool findPoduct(const char* keyName, ProductKeys** pk) const
    {
        auto it = std::find(begin(), end(), keyName);
        if (it != end())
            *pk = const_cast<ProductKeys*>(&(*it));
        return it != end();
    }

    ProductValue getPoductValue(const char* keyName) const
    {
        auto it = std::find(begin(), end(), keyName);
        if (it != end())
           return it->val;
        throw std::logic_error("Don't find the value.");
    }
};

class Products
{

public:

    Products() {};
    ~Products() {};

public:

    inline void readProducts(const std::string& path);
    inline const Product* findProduct(size_t index);

    inline const ProductsMap& getProducts();

protected:

    ProductsMap _productsMap;

};

void Products::readProducts(const std::string& path)
{
    //解析json
    Json::Value responseData;
    Json::Reader jsonReader;

    std::string workPath = boost::filesystem::initial_path<boost::filesystem::path>().string() + "\\" + path;
    std::ifstream file;
    file.open(workPath.c_str(), std::ios::in | std::ios::binary);

    try {
        jsonReader.parse(file, responseData);
    } catch (...){
        throw std::logic_error("初始化商品失败, 解析商品信息失败. \n");
    }

    auto& members = responseData.getMemberNames();
    auto& it_pro = std::find(members.begin(), members.end(), "products");

    assert(it_pro != members.end());

    char error[128] = { 0 };
    if (it_pro == members.end()) {
        sprintf_s(error, "初始化商品失败,未读取到商品信息: %s. \n", workPath.c_str());
        throw std::logic_error(error);
    }

    try {

        auto& Jsn_product = responseData["products"];

        for each (auto& val in Jsn_product) {

            int id = val["id"].asInt();
            Product proVal;

            ProductKeys pro_id;
            pro_id.key = "id";
            pro_id.val.nValue = id;
            proVal.push_back(pro_id);

            ProductKeys pro_fee;
            pro_fee.key = "total_fee";
            pro_fee.val.nValue = val["total_fee"].asInt();
            proVal.push_back(pro_fee);

            ProductKeys pro_dis;
            pro_dis.key = "discount";
            pro_dis.val.dValue = val["discount"].asDouble();
            proVal.push_back(pro_dis);

            ProductKeys pro_type;
            pro_type.key = "product_type";
            std::string& str_type = responseData["product_type"].asString();

            ProductKeys pro_des;
            pro_des.key = "body";
            std::string& str_des = responseData["body"].asString();

            ProductKeys pro_trade;
            pro_trade.key = "trade_type";
            std::string& str_trade = responseData["trade_type"].asString();

            bool is_utf8 = tool::isUTF8String(str_type.c_str());
            assert(is_utf8);
            if (!is_utf8)  throw std::logic_error("");

            is_utf8 = tool::isUTF8String(str_des.c_str());
            assert(is_utf8);
            if (!is_utf8)  throw std::logic_error("");

            is_utf8 = tool::isUTF8String(str_trade.c_str());
            assert(is_utf8);
            if (!is_utf8)  throw std::logic_error("");

            sprintf_s(pro_type.val.cStr, "%s", str_type.c_str());
            proVal.push_back(pro_type);

            sprintf_s(pro_des.val.cStr, "%s", str_des.c_str());
            proVal.push_back(pro_des);

            sprintf_s(pro_trade.val.cStr, "%s", str_trade.c_str());
            proVal.push_back(pro_trade);

            _productsMap.insert(ProductPair(id, proVal));

        }

    } catch (...)  {

        throw std::logic_error("初始化商品失败, 解析商品信息失败. \n");

    }

}

const Product* Products::findProduct(size_t index)
{
    auto it = _productsMap.find(index);
    if (it != _productsMap.end())
        return &(it->second);
    return nullptr;
}

const ProductsMap& Products::getProducts()
{
    return _productsMap;
}

}

#endif // !_PRODUCT_H_
