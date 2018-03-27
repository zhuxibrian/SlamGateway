/*
 * HxConfig.cpp
 *
 *  Created on: 2017年8月23日
 *      Author: houxd
 */

#include "HxConfig.h"

#include <stdlib.h>
#include <cstdio>

//#include "utils.h"

using namespace std;

#define HxConfigPrintf(...)	//printf(__VA_ARGS__)

#include "../libini/iniparser.h"
using namespace INI;
typedef void *INIFILE_T;

static void inifile_close(INIFILE_T ini)
{
	iniparser_freedict((dictionary*)ini);
}

static INIFILE_T inifile_open(const char *fpath, const char *mode)
{
	return (INIFILE_T)iniparser_load(fpath,mode);
}
static int inifile_entry_count(INIFILE_T ini)
{
	dictionary* d = (dictionary*)ini;
	if(d==NULL)
		return -1;
	return d->n;
}
static int inifile_get_entry(INIFILE_T ini, int index, char **sect,char **key, char **val)
{
	dictionary* d = (dictionary*)ini;
	if(d==NULL)
		return -1;
	if(index>=d->size)
		return -2;
	char *k = d->key[index];
	char *v = d->val[index];
	char *p = strchr(k,':');
	if(p==NULL){
		*sect = NULL;
		*key = k;
		*val = v;
	}else{
		int n=p-k;
		if(n==0){
			*sect = NULL;
			*key = p+1;
			*val = v;
		}else{
			static char sect_buf[64];
			memset(sect_buf,0,64);
			if(n>63)n=63;
			strncpy(sect_buf,k,n);
			*sect = sect_buf;
			*key = p+1;
			*val = v;
		}
	}
	return 0;
}
static int inifile_setstr(INIFILE_T ini, const char *section, const char *key, const char *val)
{
	int res;
	if(((dictionary*)ini)->m[0]=='r')	//readonly file.
		return -1;
	char *v = strdup(val);
	char *buf;
	if(section==NULL || section[0]==0){
		buf = new char[strlen(key)+2];
		sprintf(buf,":%s",key);
	}else{
		buf = new char[strlen(section)+strlen(key)+2];
		sprintf(buf,"%s:%s",section,key);
	}
	res = iniparser_setstr((dictionary*)ini, buf, v);
	free(v);
	delete buf;
	return res;
}
static int inifile_setstr(INIFILE_T ini, const char *key, const char *val)
{
	return inifile_setstr(ini,NULL,key,val);
}

char HxReflector::int2asc(int n)
{
    if(n<0)
        return '0';
    else if(n>=0 && n<10)
        return n+'0';
    else if(n>=10 && n<16)
        return (n-10)+'a';
    else
        return 'f';
}
char HxReflector::int2ASC(int n)
{
    if(n<0)
        return '0';
    else if(n>=0 && n<10)
        return n+'0';
    else if(n>=10 && n<16)
        return (n-10)+'A';
    else
        return 'F';
}
int HxReflector::asc2int(char asc)
{
    if(asc<'0')
        return 0;
    else if(asc>='0' && asc<='9')
        return asc-'0';
    else if(asc>='A' && asc<='F')
        return asc-'A'+10;
    else if(asc>='a' && asc<='f')
        return asc-'a'+10;
    else
        return 15;
}
uint8_t* HxReflector::hx_dumphex(const void *src,int srclen,void *buf)
{
    uint8_t *p = (uint8_t*)src;
    uint8_t *q = (uint8_t*)buf;
    for(int i=0;i<srclen;i++){
        uint8_t c = *p++;
        *q++ = int2asc((c>>4)&0x0Fu);
        *q++ = int2asc((c>>0)&0x0Fu);
    }
    return (uint8_t*)buf;
}
uint8_t* HxReflector::hx_dumpHEX(const void *src,int srclen,void *buf)
{
    uint8_t *p = (uint8_t*)src;
    uint8_t *q = (uint8_t*)buf;
    for(int i=0;i<srclen;i++){
        uint8_t c = *p++;
        *q++ = int2ASC((c>>4)&0x0Fu);
        *q++ = int2ASC((c>>0)&0x0Fu);
    }
    return (uint8_t*)buf;
}
char* HxReflector::hx_dumphex2str(const void *src,int srclen,void *buf)
{
    char *res = (char*)hx_dumphex(src,srclen,buf);
    res[srclen*2] = '\0';
    return res;
}
char* HxReflector::hx_dumpHEX2str(const void *src,int srclen,void *buf)
{
    char *res = (char*)hx_dumpHEX(src,srclen,buf);
    res[srclen*2] = '\0';
    return res;
}
// "1234" => "\12\x34"
void* HxReflector::hx_hexcode2bin(const void *hexcode, int len, void *bin)
{
    unsigned int tmp;
    int c,i;
    unsigned char *t = (unsigned char*)bin;
    const unsigned char *s = (const unsigned char *)hexcode;
    for(i=0;i<len;i+=2){
        tmp = s[i];
        c = 0x0F & asc2int(tmp);
        c<<=4;
        tmp = s[i+1];
        c += 0x0F & asc2int(tmp);
        *t++ = c;
    }
    return bin;
}

HxReflector::~HxReflector(){
	if(type)free(type);
	if(name)free(name);
	try{
		release_defval<char					>();
		release_defval<short				>();
		release_defval<int					>();
		release_defval<long					>();
		release_defval<long long			>();
		release_defval<unsigned char		>();
		release_defval<unsigned short		>();
		release_defval<unsigned int			>();
		release_defval<unsigned long		>();
		release_defval<unsigned long long	>();
		release_defval<float				>();
		release_defval<double				>();
		release_defval<long double			>();
		release_defval<bool					>();
		release_defval<string				>();
	}catch(bool b){
		HxConfigPrintf("catch b=%s\n",b?"true":"false");
	}catch(...){
		HxConfigPrintf("catch what?\n");
	}
}
void HxReflector::setValue(string val){
	try{
		set_value<char					>(val);
		set_value<short					>(val);
		set_value<int					>(val);
		set_value<long					>(val);
		set_value<long long				>(val);
		set_value_unsigned_char			 (val);
		set_value<unsigned short		>(val);
		set_value<unsigned int			>(val);
		set_value<unsigned long			>(val);
		set_value<unsigned long long	>(val);
		set_value<float					>(val);
		set_value<double				>(val);
		set_value<long double			>(val);
		set_value<bool					>(val);
		set_value<string				>(val);
	}catch(bool b){
		HxConfigPrintf("catch b=%s\n",b?"true":"false");
	}catch(...){
		HxConfigPrintf("catch what?\n");
	}
}
string HxReflector::getValue()const{
	string res;
	try{
		get_value<char					>(res);
		get_value<short					>(res);
		get_value<int					>(res);
		get_value<long					>(res);
		get_value<long long				>(res);
		get_value_unsigned_char			 (res);
		get_value<unsigned short		>(res);
		get_value<unsigned int			>(res);
		get_value<unsigned long			>(res);
		get_value<unsigned long long	>(res);
		get_value<float					>(res);
		get_value<double				>(res);
		get_value<long double			>(res);
		get_value<bool					>(res);
		get_value<string				>(res);
	}catch(bool b){
		HxConfigPrintf("catch b=%s\n",b?"true":"false");
	}catch(...){
		HxConfigPrintf("catch what?\n");
	}
	return res;
}
bool HxReflector::isEnd() const
{
	if(type&&name&&def_val&&member)
		return false;
	return true;
}
void* HxReflector::calc_align_addr(void* addr, int offset, int n)
{
	unsigned long p = ((unsigned long)addr) + offset;
	if(p%n){
		return (void*)(p/n*n+n);
	}else{
		return (void*)p;
	}
}
string HxReflector::getDefVal()const{
	string res;
	try{
		get_def_value<char					>(res);
		get_def_value<short					>(res);
		get_def_value<int					>(res);
		get_def_value<long					>(res);
		get_def_value<long long				>(res);
		get_def_value_unsigned_char			 (res);
		get_def_value<unsigned short		>(res);
		get_def_value<unsigned int			>(res);
		get_def_value<unsigned long			>(res);
		get_def_value<unsigned long long	>(res);
		get_def_value<float					>(res);
		get_def_value<double				>(res);
		get_def_value<long double			>(res);
		get_def_value<bool					>(res);
		get_def_value<string				>(res);
	}catch(bool b){
		HxConfigPrintf("catch b=%s\n",b?"true":"false");
	}catch(...){
		HxConfigPrintf("catch what?\n");
	}
	return res;
}
bool HxConfig::Load(const char *filePath)
{
	if(filePath==NULL)
		return false;
	INIFILE_T ini = inifile_open(filePath,"r");
	int inic = ini?inifile_entry_count(ini):0;
	HxReflector* p = (HxReflector*)this;
	if(p->getMember()==NULL){
		HxConfigPrintf("no member!\n");
		return false;
	}
	do{
		char *s,*k,*v;
		int i;
		for(i=0;i<inic;i++){
			inifile_get_entry(ini,i,&s,&k,&v);
			if(strcmp(k,p->getName())==0){
				p->setValue(v);
				break;
			}
		}
		if(i==inic)
			p->setValue(p->getDefVal());
		p = p->getNext();
	}while(!p->isEnd());
	inifile_close(ini);
	return true;
}

bool HxConfig::Save(const char *filePath)
{
	if(filePath==NULL)
		return false;
	INIFILE_T ini = inifile_open(filePath,"w+");
	if(ini==NULL)
		return false;
	HxReflector* p = (HxReflector*)this;
	if(p->getMember()==NULL){
		HxConfigPrintf("no member\n");
		return false;
	}
	do{
		inifile_setstr(ini,p->getName(),p->getValue().c_str());
		p = p->getNext();
	}while(!p->isEnd());
	inifile_close(ini);
	return true;
}

void hxConfig_test()
{
	struct Conf:HxConfig {
			HX_OPTION_VALUE(string,book,"fuck")
			HX_OPTION_VALUE(int,book_count,71)
			HX_OPTION_VALUE(char,book_c,'x')
			HX_OPTION_VALUE(vector<int>,vv,{78,1,9,3})
			HX_OPTION_VALUE(bool,save,false)
			HX_OPTION_VALUE(string,good,"9988")
		HX_OPTION_END()
	};
	Conf cf;
	cf.Load("/opt/a.ini");
	cf.Save("/opt/a.ini");

	printf("book=%s\n",cf.book.c_str());
	printf("book_count=%d\n",cf.book_count);
	printf("book_c=%c\n",cf.book_c);
	printf("vv=%d\n",cf.vv[0]);
	printf("vv=%d\n",cf.vv[1]);
	printf("vv=%d\n",cf.vv[2]);
	printf("vv=%d\n",cf.vv[3]);
	printf("save=%d\n",cf.save);
	printf("good=%s\n",cf.good.c_str());

}


