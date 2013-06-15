#ifndef _INCLUDE_SCENECLASS
#define _INCLUDE_SCENECLASS

//��ʐ؂�ւ��p�e���v���[�g�N���X
template<class type>
class Scene{
private:
	void (*m_init)(type);		//
	void (*m_update)(type);		//	�֐��ւ̃|�C���^
	void (*m_draw)(type);		//
	void (*m_delete)(type);		//
public:
	void Set( void (*_init)(type), void (*_update)(type), void (*_draw)(type), void (*_delete)(type) );
	void Init(type);
	void Update(type);
	void Draw(type);
	void Delete(type);
};

//���ꂼ��̊֐����Z�b�g
template<class type>void Scene<type>::Set( void (*_init)(type), void (*_update)(type), void (*_draw)(type), void (*_delete)(type) ){
	m_init = _init;
	m_update = _update;
	m_draw = _draw;
	m_delete = _delete;
}


//�Z�b�g���ꂽ�֐������s
template<class type>void Scene<type>::Init(type data){
	m_init(data);
}
template<class type>void Scene<type>::Update(type data){
	m_update(data);
}
template<class type>void Scene<type>::Draw(type data){
	m_draw(data);
}
template<class type>void Scene<type>::Delete(type data){
	m_delete(data);
}


#endif