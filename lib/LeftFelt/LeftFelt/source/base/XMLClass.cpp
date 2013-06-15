#include <base/XMLClass.h>

XML::XML(String source){
	this->tag_list = Create(source);
}

TagList XML::Create(String &source){
	TagList tag_list;
	Tag tag;

	//タブを半角スペースに置換
	source.Replace("\t"," ");
	//先頭の空白を除去
	source.FrontStrip(' ');

	String data = "";
	//先頭から読み込む
	for(int i = 0 ; i <(signed)source.size() ; i++){
		switch(source.at(i)){
			case '<':
				if(data != ""){
					tag.setText(data);
					tag_list.push_back(tag);
					tag.clear();
					data = "";
				}
				continue;
			case '>':
				//タグの終了
				if(data != ""){
					this->setTag(tag,data);
					tag_list.push_back(tag);
					tag.clear();
					data = "";
				}
				continue;
			default:
				data += source.at(i);
				break;
		}
	}
	return tag_list;
}

/*
タグを設定する
@param Tag tag
@param String source : <と>の間の文字列
*/
void XML::setTag(Tag &tag, String source){
	StringList source_list;
	//先頭の空白を除去
	source.FrontStrip();
	//決めうちで最初の要素をタグ名にする
	source_list = source.FrontSplit(' ');
	tag.setName(source_list[0]);
	//プロパティを設定
	this->setProperty(tag, source_list[1]);
}
//プロパティを設定する
void XML::setProperty(Tag &tag, String source){
	StringList result;
	String name;
	String value;

	while(source != ""){
		name = "";
		value = "";
		result.clear();

		source.FrontStrip(' ');
		result = source.FrontSplit('=');
		source = result[1];
		result[0].EndStrip(' ');
		result = result[0].FrontSplit(' ');
		if(result[1] != ""){
			tag.setProperty(result[0],value);
			source = result[1]+source;
			continue;
		}
		//名前
		name = result[0];
		//値
		if(source != ""){
			source.FrontStrip('=');
			source.FrontStrip(' ');
			result = source.FrontSplit('\"');
			if(result[1] == ""){
				result = source.FrontSplit('\'');
				if(result[1] != ""){
					//シングルクォーテーションで囲われている
					result[1].FrontStrip('\'');
					result = result[1].FrontSplit('\'');
					result[1].FrontStrip('\'');
				}
			}else{
				//ダブルクォーテーションで囲われている
				result[1].FrontStrip('\"');
				result = result[1].FrontSplit('\"');
				result[1].FrontStrip('\"');
			}
			value = result[0];
			source = result[1];
		}
		tag.setProperty(name,value);
	}
}

String XML::setPropertyName(Tag &tag, String &source){
	StringList result;

	source.FrontStrip(' ');
	result = source.FrontSplit('=');
	result[0].EndStrip(' ');
	result[0].Split(' ');



	source = result[1];
	return result[0];
}

String XML::setPropertyValue(Tag &tag, String &source){
	return "";
}

//タグ名で検索
TagList XML::getElementsByTagName(String tag_name){
	TagList tag_list;
	for(int i = 0 ; i < (signed)this->tag_list.size() ; i++){
		if(this->tag_list[i].getName() == tag_name){
			tag_list.push_back(this->tag_list[i]);
		}
	}

	return tag_list;
}