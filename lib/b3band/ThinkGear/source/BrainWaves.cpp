#include <BrainWaves.h>

BrainWaves::BrainWaves(){
}

BrainWaves::BrainWaves(std::map<std::string, double> waves){
	std::map<std::string,double>::iterator itr;
	this->waves = waves;
	for(itr = this->waves.begin() ; itr != this->waves.end() ; itr++){
		this->parameters.push_back(itr->first);
	}
}

std::vector<std::string> BrainWaves::getParameters(){
	return this->parameters;
}

double BrainWaves::getValue(std::string name){
	return this->waves[name];
}

std::map<std::string,double> BrainWaves::toMap(){
	return this->waves;
}

//指定された定義済データ型の値で返す
double BrainWaves::getBattery(){
	return this->getValue("battery");
}
double BrainWaves::getPoorSignal(){
	return this->getValue("poor_signal");
}
double BrainWaves::getAttention(){
	return this->getValue("attention");
}
double BrainWaves::getMeditation(){
	return this->getValue("meditation");
}
double BrainWaves::getRaw(){
	return this->getValue("raw");
}
double BrainWaves::getDelta(){
	return this->getValue("delta");
}
double BrainWaves::getTheta(){
	return this->getValue("theta");
}
double BrainWaves::getAlpha1(){
	return this->getValue("alpha1");
}
double BrainWaves::getAlpha2(){
	return this->getValue("alpha2");
}
double BrainWaves::getBeta1(){
	return this->getValue("beta1");
}
double BrainWaves::getBeta2(){
	return this->getValue("beta2");
}
double BrainWaves::getGamma1(){
	return this->getValue("gamma1");
}
double BrainWaves::getGamma2(){
	return this->getValue("gamma2");
}
double BrainWaves::getBlinkStrength(){
	return this->getValue("blink_strength");
}

BrainWaves BrainWaves::operator+(BrainWaves brain_waves){
	std::for_each(this->parameters.begin(), this->parameters.end(),[&](std::string key){
		brain_waves.waves[key] = brain_waves.waves[key] + this->waves[key];
	});
	return waves;
}
BrainWaves BrainWaves::operator-(BrainWaves brain_waves){
	std::for_each(this->parameters.begin(), this->parameters.end(),[&](std::string key){
		brain_waves.waves[key] = brain_waves.waves[key] - this->waves[key];
	});
	return waves;
}
BrainWaves BrainWaves::operator*(BrainWaves brain_waves){
	std::for_each(this->parameters.begin(), this->parameters.end(),[&](std::string key){
		brain_waves.waves[key] = brain_waves.waves[key] * this->waves[key];
	});
	return waves;
}
BrainWaves BrainWaves::operator/(BrainWaves brain_waves){
	std::for_each(this->parameters.begin(), this->parameters.end(),[&](std::string key){
		brain_waves.waves[key] = brain_waves.waves[key] / this->waves[key];
	});
	return waves;
}

BrainWaves BrainWaves::operator+=(BrainWaves waves){
	*this = *this + waves;
	return *this;
}
BrainWaves BrainWaves::operator-=(BrainWaves waves){
	*this = *this - waves;
	return *this;
}
BrainWaves BrainWaves::operator*=(BrainWaves waves){
	*this = *this * waves;
	return *this;
}
BrainWaves BrainWaves::operator/=(BrainWaves waves){
	*this = *this / waves;
	return *this;
}