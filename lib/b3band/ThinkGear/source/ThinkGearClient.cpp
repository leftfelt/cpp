#include <ThinkGearClient.h>

ThinkGearClient::ThinkGearClient(){
	this->connection_id = -1;
	this->is_start_capture = false;
	this->create();
	this->waves_map["alpha1"]			= TG_DATA_ALPHA1;
	this->waves_map["alpha2"]			= TG_DATA_ALPHA2;
	this->waves_map["beta1"]			= TG_DATA_BETA1;
	this->waves_map["beta2"]			= TG_DATA_BETA2;
	this->waves_map["gamma1"]			= TG_DATA_GAMMA1;
	this->waves_map["gamma2"]			= TG_DATA_GAMMA2;
	this->waves_map["delta"]			= TG_DATA_DELTA;
	this->waves_map["theta"]			= TG_DATA_THETA;
	this->waves_map["raw"]				= TG_DATA_RAW;
	this->waves_map["blink_strength"]	= TG_DATA_BLINK_STRENGTH;
	this->waves_map["attention"]		= TG_DATA_ATTENTION;
	this->waves_map["meditation"]		= TG_DATA_MEDITATION;
	this->waves_map["poor_signal"]		= TG_DATA_POOR_SIGNAL;
	this->waves_map["battery"]			= TG_DATA_BATTERY;

}
ThinkGearClient::~ThinkGearClient(){
}

//�p�P�b�g�̎����ǂݎ���L���ɂ���
int ThinkGearClient::enableAutoRead(bool enabled){
	int errCode;
	errCode = TG_EnableAutoRead(this->connection_id, enabled);
	if(errCode == -1){
		throw std::exception("connectionId does not refer to a valid ThinkGearClient Connection ID handle");
	}else if(errCode == -2){
		throw std::exception("unable to enable background auto-reading");
	}else if(errCode == -3){
		throw std::exception("an error occurs while attempting to disable background auto-reading");
	}
	return errCode;
}

//�L���v�`�����ł��邩
bool ThinkGearClient::isStartCapture(){
	return this->is_start_capture;
}

//�V����ID�����蓖�Ă�B
bool ThinkGearClient::create(){
	this->connection_id = TG_GetNewConnectionId();
    if( this->connection_id == -1 ) {
		throw std::exception("too many Connections have been created without being freed by TG_FreeConnection()");
    }else if( this->connection_id == -2 ) {
		throw std::exception("there is not enough free memory to allocate to a new ThinkGearClient Connection");
	}
	return true;
}

//�w�肵���V���A���|�[�g�ɐڑ����s���AID�Ɗ֘A�t����
int ThinkGearClient::connect(std::string port_name, int bit_rate, int data_type){
	int errCode;
	std::string comPortName = "\\\\.\\";
    comPortName += port_name;
    errCode = TG_Connect( this->connection_id, 
                          comPortName.c_str(), 
                          TG_BAUD_9600, 
                          TG_STREAM_PACKETS );
	if(errCode == -1){
		throw std::exception("connectionId does not refer to a valid ThinkGearClient Connection ID handle");
	}else if(errCode == -2){
		throw std::exception("serialPortName could not be opened as a serial communication port for any reason. Check that the name is a valid COM port on your system");
	}else if(errCode == -3){
		throw std::exception("serialBaudrate is not a valid TG_BAUD_* value");
	}else if(errCode == -4){
		throw std::exception("serialDataFormat is not a valid TG_STREAM_* type");
	}
    return errCode;
}

//ID���ڑ����Ă���V���A���ʐM�|�[�g����ؒf����
void ThinkGearClient::disconnect(){
	//ID�Ɋ֘A�t�����Ă��邷�ׂẴ��������J������
	TG_FreeConnection( this->connection_id );
	TG_Disconnect(this->connection_id);
}

//�ǂݎ�����o�C�g�����O�t�@�C���֎����I�ɋL�^����
int ThinkGearClient::streamLog(std::string filename){
	int errCode;
	errCode = TG_SetStreamLog( this->connection_id, filename.c_str() );
	if(errCode == -1){
		throw std::exception("connectionId does not refer to a valid ThinkGearClient Connection ID handle");
	}else if(errCode == -2){
		throw std::exception("filename could not be opened for writing. You may check errno for the reason");
	}
	return errCode;
}

//��̓f�[�^�����O�t�@�C���֎����I�ɋL�^����
int ThinkGearClient::dataLog(std::string filename){
	int errCode;
	errCode = TG_SetDataLog( this->connection_id, filename.c_str());
	if(errCode == -1){
		throw std::exception("connectionId does not refer to a valid ThinkGearClient Connection ID handle");
	}else if(errCode == -2){
		throw std::exception("filename could not be opened for writing. You may check errno for the reason");
	}
	return errCode;
}

//�w�肵���p�P�b�g�������ǂݎ��
int ThinkGearClient::readPackets(int packet_num){
	int read_packet_num;
	read_packet_num = TG_ReadPackets( this->connection_id, packet_num );
	if(read_packet_num == -1){
		throw std::exception("connectionId does not refer to a valid ThinkGearClient Connection ID handle");
	}else if(read_packet_num == -2){
		throw std::exception("there were not even any bytes available to be read from the Connection's serial communication stream");
	}else if(read_packet_num == -3){
		throw std::exception("an I/O error occurs attempting to read from the Connection's serial communication stream");
	}
	return read_packet_num;
}

//�p�P�b�g�̓ǂݎ����J�n����
void ThinkGearClient::startCapture(){
	this->is_start_capture = true;
	this->enableAutoRead(true);
}

//�p�P�b�g�̓ǂݎ����~����
void ThinkGearClient::stopCapture(){
	this->is_start_capture = false;
	this->enableAutoRead(false);
}

//�w�肳�ꂽ��`�σf�[�^�^�̒l��Ԃ�
double ThinkGearClient::getCaptureData(int data_type){
	if(this->isStartCapture()){
		//�p�P�b�g�̎����Ǎ����Ȃ炻���炩��l���擾����
		return (double)TG_GetValue(this->connection_id, data_type);
	}
	return this->getValue(data_type);
}

//�p�P�b�g���P�ǂݍ��݁A�w�肳�ꂽ��`�σf�[�^�^�̒l�ŕԂ�
double ThinkGearClient::getValue(int data_type){
	int errCode;
	//�p�P�b�g���P�ǂݍ���
	errCode = TG_ReadPackets( this->connection_id, 1 );
	if(errCode == 1){
		if( TG_GetValueStatus(this->connection_id, data_type) != 0 ) {
			return (double)TG_GetValue(this->connection_id, data_type);
		}
	}
	return (double)0;
}

BrainWaves ThinkGearClient::getWaves(){
	std::map<std::string,double> waves;
	std::for_each(this->waves_map.begin(),this->waves_map.end(),[&](std::pair<std::string,int> itr){
		waves[itr.first] = this->getCaptureData(itr.second);
	});

	return BrainWaves(waves);
}
