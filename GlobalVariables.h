#pragma once
#include <variant>
#include <string>
#include <map>
#include <Vector3.h>

//グローバル変数
class GlobalVariables {
public:
	static GlobalVariables* GetInstance();

	//毎フレーム処理
	void Update();

	//グループの作成
	void CreateGroup(const std::string& groupName);

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

	//項目
	struct Item {
		//項目の値
		std::variant<int32_t, float, Vector3> value;

	};

	//グループ
	struct Group {
		std::map<std::string, Item> items;
	};

	//全データ
	std::map<std::string, Group> datas_;

};
