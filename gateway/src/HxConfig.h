/*
 * HxConfig.h
 *
 *  Created on: 2017年8月23日
 *      Author: houxd
 *  ==========================================================
 *  Demo Usage:
 *
	struct Config : HxConfig {
		HX_OPTION_VALUE(std::string,book,"houxd")
		HX_OPTION_VALUE(int,book_count,3)
		HX_OPTION_VALUE(vector<int>,array,{78,1,9,3})
		HX_OPTION_END()    // !!! must define HX_OPTION_END() !!!
	};
	Config config;
	const char *fpath = "/opt/a.ini";
	config.Load(fpath);
	printf("book is %s\n",config.book.c_str());
	printf("book_count is %d\n",config.book_count);
	config.book_count.val--;
	config.Save(fpath);

 *
 *  Supported Types:
 *	char,short,int,long,long long,unsigned char,
 *	unsigned short,unsigned int,unsigned long,unsigned long long,
 *	float,double,long double,bool,string,
 *	vector<T> surpported, hehe.
 *	And self defined simple type can be supported yet.
 *  =========================================================
 */

#ifndef HXCONFIG_H_
#define HXCONFIG_H_

#include <sys/types.h>
#include <cstring>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <cstdio>
#include <cstdint>

struct HxReflector {
private:
	static char int2asc(int n);
	static char int2ASC(int n);
	static int asc2int(char asc);
	static uint8_t* hx_dumphex(const void *src,int srclen,void *buf);
	static uint8_t* hx_dumpHEX(const void *src,int srclen,void *buf);
	static char* hx_dumphex2str(const void *src,int srclen,void *buf);
	static char* hx_dumpHEX2str(const void *src,int srclen,void *buf);
	static void* hx_hexcode2bin(const void *hexcode, int len, void *bin);
protected:
	char *type;
	char *name;
	void *def_val;
	void *tag;
	void *member;
	HxReflector *next;
	template<typename T>
	HxReflector(
			const char *typeinfoName,
			const char *valueName,
			const T &defaultVelue//must be special
			)
		:type(strdup(typeinfoName==0?"":typeinfoName)),
		 name(strdup(valueName==0?"":valueName)),
		 def_val(new T(defaultVelue)),
		 tag(0)
    {
		member = calc_align_addr(this,sizeof(HxReflector),__alignof__(T));
		next = (HxReflector*)calc_align_addr(member,sizeof(T),__alignof__(HxReflector));
	}
	template<typename T>
	HxReflector(
			void *tagPtr,
			const char *typeinfoName,
			const char *valueName,
			const T &defaultVelue//must be special
			)
		 :type(strdup(typeinfoName==0?"":typeinfoName)),
		 name(strdup(valueName==0?"":valueName)),
		 def_val(new T(defaultVelue)),
		 tag(tagPtr)
	{
		member = calc_align_addr(this,sizeof(HxReflector),__alignof__(T));
		next = (HxReflector*)calc_align_addr(member,sizeof(T),__alignof__(HxReflector));
	}
	HxReflector()	/* use for end */
		:type(0),name(0),def_val(0),tag(0),member(0),next(0){}
	~HxReflector();
	static void *calc_align_addr(void *addr,int offset,int n);
	template<typename T>
	void release_defval(){
		if(!type) return;//this is HX_OPTION_END
		if(strcmp(typeid(T).name(),type)==0){
			delete (T*)def_val;
			throw true;
		}
		if(strcmp(typeid(std::vector<T>).name(),type)==0){
			delete (std::vector<T>*)def_val;
			throw true;
		}
	}
	template<typename T>
	void set_value(std::string val){
		if(strcmp(typeid(T).name(),type)==0){
			std::stringstream ss(val);
			ss>>*((T*)member);
			throw true;
		}
		if(strcmp(typeid(std::vector<T>).name(),type)==0){
			std::vector<T> &v = *(std::vector<T>*)member;
			v.clear();
			std::stringstream ss(val);
			for(;;){
				T vo;
				ss>>vo;
				if(ss.fail())
					break;
				v.push_back(vo);
			}
			throw true;
		}
	}
	void set_value_unsigned_char(std::string val){
		typedef unsigned char T;
		std::string buf;
		if(strcmp(typeid(T).name(),type)==0){
			std::stringstream ss(val);
			ss>>buf;
			hx_hexcode2bin(buf.c_str(),2,member);
			throw true;
		}
		if(strcmp(typeid(std::vector<T>).name(),type)==0){
			std::vector<T> &v = *(std::vector<T>*)member;
			v.clear();
			std::stringstream ss(val);
			for(;;){
				T vo;
				ss>>buf;
				if(ss.fail())
					break;
				hx_hexcode2bin(buf.c_str(),2,&vo);
				v.push_back(vo);
			}
			throw true;
		}
	}
	template<typename T>
	void get_value(std::string &res) const{
		if(strcmp(typeid(T).name(),type)==0){
			std::stringstream ss;
			ss<<*((T*)member);
			res = ss.str();
			throw true;
		}
		if(strcmp(typeid(std::vector<T>).name(),type)==0){
			std::vector<T> &v = *(std::vector<T>*)member;
			std::stringstream ss;
			for(uint i=0;i<v.size();i++){
				ss<<v[i]<<" ";
			}
			res = ss.str();
			throw true;
		}
	}
	void get_value_unsigned_char(std::string &res) const{
		typedef unsigned char T;
		char buf[3];
		if(strcmp(typeid(T).name(),type)==0){
			std::stringstream ss;
			ss<<hx_dumphex2str(member,1,buf);
			res = ss.str();
			throw true;
		}
		if(strcmp(typeid(std::vector<T>).name(),type)==0){
			std::vector<T> &v = *(std::vector<T>*)member;
			std::stringstream ss;
			for(uint i=0;i<v.size();i++){
				ss<<hx_dumphex2str(&v[i],1,buf)<<" ";
			}
			res = ss.str();
			throw true;
		}
	}
	template<typename T>
	void get_def_value(std::string &res) const{
		if(strcmp(typeid(T).name(),type)==0){
			std::stringstream ss;
			ss<<*((T*)def_val);
			res = ss.str();
			throw true;
		}
		if(strcmp(typeid(std::vector<T>).name(),type)==0){
			std::vector<T> &v = *(std::vector<T>*)def_val;
			std::stringstream ss;
			for(uint i=0;i<v.size();i++){
				ss<<v[i]<<" ";
			}
			res = ss.str();
			throw true;
		}
	}
	void get_def_value_unsigned_char(std::string &res) const{
		typedef unsigned char T;
		char buf[3];
		if(strcmp(typeid(T).name(),type)==0){
			std::stringstream ss;
			ss<<hx_dumphex2str(def_val,1,buf);
			res = ss.str();
			throw true;
		}
		if(strcmp(typeid(std::vector<T>).name(),type)==0){
			std::vector<T> &v = *(std::vector<T>*)def_val;
			std::stringstream ss;
			for(uint i=0;i<v.size();i++){
				ss<<hx_dumphex2str(&v[i],1,buf)<<" ";
			}
			res = ss.str();
			throw true;
		}
	}
public:
	/*
	 * get next reflector ponter.
	 */
	HxReflector* getNext()const{
		return next;
	}
	/*
	 * check the reflector is end of all elements
	 */
	bool isEnd()const;
	/*
	 * set member val by string
	 */
	void setValue(std::string val);
	/*
	 * get member val to string
	 */
	std::string getValue()const;
	/*
	 * get default val to string
	 */
	std::string getDefVal()const;
	/*
	 * get member addr pointer
	 */
	const void* getMember() const {
		return member;
	}
	/*
	 * get member name
	 */
	const char* getName() const {
		return name;
	}
	/*
	 * get member type
	 */
	const char* getType() const {
		return type;
	}
	/*
	 * get member tag, use bind self defined data type instance
	 */
	const void* getTag() const {
		return tag;
	}
};

struct HxConfig {
	bool Load(const char *filePath);
	bool Save(const char *filePath);
};

#define HX_OPTION_VALUE(Type,Name,Default...) \
	struct z_##Name##reflect_info_class : HxReflector { \
		z_##Name##reflect_info_class():HxReflector(typeid(Type).name(),#Name,Type(Default)){} \
	} Name##reflect_info; \
	Type Name ;

#define HX_OPTION_VALUE_ETX(Tag,Type,Name,Default...) \
	struct z_##Name##reflect_info_class : HxReflector { \
		z_##Name##reflect_info_class():HxReflector((void*)(Tag),typeid(Type).name(),#Name,Type(Default)){} \
	} Name##reflect_info; \
	Type Name ;

#define HX_OPTION_END() \
	struct z_##Name##reflect_info_class : HxReflector { \
		z_##Name##reflect_info_class():HxReflector(){} \
	} Name##reflect_info; \

#endif /* HXCONFIG_H_ */
