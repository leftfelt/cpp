#include <base/XMLClass.h>

XML::XML(String source){
	this->tag_list = Create(source);
}

TagList XML::Create(String &source){
	TagList tag_list;
	Tag tag;

	//�^�u�𔼊p�X�y�[�X�ɒu��
	source.Replace("\t"," ");
	//�擪�̋󔒂�����
	source.FrontStrip(' ');

	String data = "";
	//�擪����ǂݍ���
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
				//�^�O�̏I��
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
�^�O��ݒ肷��
@param Tag tag
@param String source : <��>�̊Ԃ̕�����
*/
void XML::setTag(Tag &tag, String source){
	StringList source_list;
	//�擪�̋󔒂�����
	source.FrontStrip();
	//���߂����ōŏ��̗v�f���^�O���ɂ���
	source_list = source.FrontSplit(' ');
	tag.setName(source_list[0]);
	//�v���p�e�B��ݒ�
	this->setProperty(tag, source_list[1]);
}
//�v���p�e�B��ݒ肷��
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
		//���O
		name = result[0];
		//�l
		if(source != ""){
			source.FrontStrip('=');
			source.FrontStrip(' ');
			result = source.FrontSplit('\"');
			if(result[1] == ""){
				result = source.FrontSplit('\'');
				if(result[1] != ""){
					//�V���O���N�H�[�e�[�V�����ň͂��Ă���
					result[1].FrontStrip('\'');
					result = result[1].FrontSplit('\'');
					result[1].FrontStrip('\'');
				}
			}else{
				//�_�u���N�H�[�e�[�V�����ň͂��Ă���
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

//�^�O���Ō���
TagList XML::getElementsByTagName(String tag_name){
	TagList tag_list;
	for(int i = 0 ; i < (signed)this->tag_list.size() ; i++){
		if(this->tag_list[i].getName() == tag_name){
			tag_list.push_back(this->tag_list[i]);
		}
	}

	return tag_list;
}