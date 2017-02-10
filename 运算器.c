#include<stdio.h>
#include<stdlib.h> 
#define NaN 0x7F800001 //非数的宏定义 

typedef unsigned int word;
typedef unsigned long int word32; 
typedef unsigned long long int word64;
typedef unsigned char byte; //8-bit
typedef unsigned short zije; //16-bit
typedef unsigned long dwrd; //32-bit

union{
	float f;
	long i;
}u, v, w, z1, z2, z3, z4;

dwrd fadd(dwrd a, dwrd b);
dwrd fsub(dwrd a, dwrd b);
dwrd fmul(dwrd a, dwrd b);
dwrd fdiv(dwrd a, dwrd b);
word64 mmdiv(word64 a,dwrd b); //整数除法运算 

word64 atom(char* s); 
void mmul(word32 i,word32 j); 
void mdiv(word64 i,word32 j); 
void madd(word a, word b);
void msub(word a, word b);
int main(void){
    word32 i,j,s,ri;
    char s1[80],s2[80],c;
    char* S;
    int choice;
    
    printf("输入0进入整数运算模式，输入1进入浮点数计算模式\n");
    scanf("%d",&choice);
    getchar();
    if(choice==0){
    ri=0;
    while(1){
        i=0;
        j=0;
        printf("What function would you like to call?\n");
        printf("a.mmul\n");
        printf("b.mdiv\n");
        printf("c.madd\n");
        printf("d.msub\n");
        printf("0.exit\n");
        if(ri!=0)
            getchar();
        ri=1;
        scanf("%c",&c);//读入选择 
        if(c=='0')
            break;
        switch(c){
            //乘法运算 
            case 'a':scanf("%s%s",s1,s2);
                     i=atom(s1);
                     j=atom(s2);
                     mmul(i,j);
                     break;
            //除法运算 
            case 'b':scanf("%s%s",s1,s2);
                     i=atom(s1);
                     j=atom(s2);
                     mdiv(i,j);
                     break;
            //加法运算 
            case 'c':scanf("%s%s",s1,s2);
                     i=atom(s1);
                     j=atom(s2);
                     madd(i,j);
                     break;
            //减法运算 
            case 'd':scanf("%s%s",s1,s2);
                     i=atom(s1);
                     j=atom(s2);
                     msub(i,j);
                     break;
        }
    }
    }
    
    if(choice==1){
   	for(;;){
		scanf("%f", &u.f);
		scanf("%f", &v.f);
	
		z1.i = fadd(u.i, v.i);
		z2.i = fsub(u.i, v.i);
		z3.i = fmul(u.i, v.i);
		z4.i = fdiv(u.i, v.i);
	
		printf("add:\n%70.40f\n%70.40f\n\n", z1.f, u.f+v.f);
		printf("sub:\n%70.40f\n%70.40f\n\n", z2.f, u.f-v.f);
		printf("mul:\n%70.40f\n%70.40f\n\n", z3.f, u.f*v.f);
		printf("div:\n%70.40f\n%70.40f\n\n", z4.f, u.f/v.f);
		
		if(getch()==27)break; //回车结束 
	}   	
    }
    return 0;
}

word64 atom(char* s){
    word32 flag,k;
    word64 i; 
    flag=0;
    k=0;
    i=0;
    if(s[k]=='-'){
        flag=1;
        k++;
    }
    while(s[k]!=0){
        i=i*10+s[k]-'0';
        k++;
    }
    if(flag==1){
        i=~i+1;
    }
    return i;
}
     
void mmul(word32 i,word32 j){
    word32 k,x,y,z,m,n,j1,j2,j3,J,s1,s2;
    n=1;
    j1=~j+1;
    j2=j+j;
    j3=j1+j1;
    y=i&0x00001;
    n=n<<1;
    if(i&n)
        x=1;    
    else
        x=0;
    m=~x+1+~x+1+y;
    s1=0;
    s2=0;
    if(m==0xFFFFFFFE)
        s1=j3+s1;
    else if(m==0xFFFFFFFF)
        s1=j1+s1;
    else if(m==0)
        s1=0;
    else if(m==1)
        s1=s1+j;
    else if(m==2)
        s1=s1+j2;
    s2=(s1&0x00000003)<<30;  
    if(s1&0x10000000){  
        s1=s1>>2;
        s1=s1|0xc0000000;
    } 
    else
        s1=s1>>2;
    for(k=1;k<16;k++){
        if(i&n)
            z=1;
        else
            z=0;
        n=n<<1;
        if(i&n)
            y=1;
        else
            y=0;
        n=n<<1;
        if(i&n)
            x=1;
        else
            x=0;
        m=~x+1+~x+1+y+z;
        if(m==0xFFFFFFFE)
            s1=j3+s1;
        else if(m==0xFFFFFFFF)
            s1=j1+s1;
        else if(m==0)
            s1=s1;
        else if(m==1)
            s1=s1+j;
        else if(m==2)
            s1=s1+j2;
        s2=s2>>2;   
        s2=(s1&3)<<30|s2;    
        if(s1&0x10000000){  
            s1=s1>>2;
            s1=s1|0xc0000000;
        } 
        else
            s1=s1>>2;    
    }  
    printf("%.8x %.8x\n",s1,s2);
} 
void mdiv(word64 i,word32 j){
    word32 flagi,flagj,m,n,k; 
    word64 I,J,j1;
    flagi=0;
    flagj=0;
    if(i>>31==1)
        flagi=1;
    if(j>>31==1)
        flagj=1;
    I=i;
    J=j;
    if(flagi)
        I=~(i-1);
    if(flagj)
        J=~(j-1);
    j1=~J+1;
    n=1;
    for(k=0;k<16;k++){//若是64位除以32位则所有16改为32 
        I=I<<1;
        if(n==1){
            I=I+(j1<<16);
        }
        else{
            I=I+(J<<16);
        }
        if(I&0x80000000)
            n=0;
        else
            n=1;
        I=I|n;
    }
    if(flagi==flagj)
        printf("%.4x\n",I&0x0000FFFF);
    else
        printf("-%.4x\n",(~(~(I&0x0000FFFF)+1)&0x0000FFFF)+1);
    if(flagi==0)
        printf("%.4x\n",I>>16);
    else
        printf("-%.4x\n",(~(~(I>>16)+1)&0x0000FFFF)+1);
}                                                                   

void madd(word a, word b){
    printf("%.8x\n",a + b);
}

void msub(word a, word b){
    printf("%.8x\n",a - b);
}

dwrd fadd(dwrd a, dwrd b){
	int  e1, e2, etemp;
	dwrd m1, m2, mtemp;
	dwrd result;
	
	e1=(a>>23)&0xFF; //得到阶码
	m1=(a&0x7FFFFF)|0x800000;  //得到尾数（整数部分1已经添上去了的那种） 
	printf("操作数1 -> 阶码：%u -- 尾数：%lX\n", e1, m1); 
	e2=(b>>23)&0xFF;
	m2=(b&0x7FFFFF)|0x800000;
	printf("操作数2 -> 阶码：%u -- 尾数：%lX\n", e2, m2);
	
	if((e1==255)&&(m1==0)&&(e2==255)&&(m2==0)&&((a^b)>>31==1)) //异号无穷大相加即结果为非数 
	    return NaN;
	if((e1==255)&&(m1==0)) //同号相加，一个数为无穷大即结果为无穷大 
		return a;
	if((e2==255)&&(m2==0))
		return b;
	if((e1==255)&&(m1!=0)) //一个数为非数即结果为非数 
		return a;
	if((e2==255)&&(m2!=0))
		return b;
		
	//判断两数绝对值大小，并用e1,m1表示绝对值大的数，e2,m2表示绝对值小的数 
	if(m1>m2);   
	if(m1<m2){
		mtemp=m1;
		m1=m2;
		m2=mtemp;
		etemp=e1;
		e1=e2;
		e2=etemp;
	}
	if((m1==m2)&&(e1>=e2));
	if((m1==m2)&&(e1<e2)){
		etemp=e1;
		e1=e2;
		e2=etemp;
	}
		
	m2>>=(e1-e2); //得到阶码差值并将m2调整至与m1同阶 
	if((a^b)>>31)m1-=m2; //判断符号，异号相减，同号相加 
	else m1+=m2;

	while(m1&0xFF000000){m1>>=1; e1++;} //如果加减后结果不是标准形式，进行调整 
	while(m1&0x00800000==0){m1<<=1; e1--;} //如果加减后结果不是标准形式，进行调整
	printf("结果 -> 阶码：%u -- 尾数：%lX\n", e1, m1);
	result=((long)e1<<23)|(m1&0x7FFFFF)|((a^b)&0x80000000);
			
	return result;
}

dwrd fsub(dwrd a, dwrd b){
	dwrd result;
	
	b=b+0x80000000;
	result=fadd(a, b);
	
	return result;
}

dwrd fmul(dwrd a, dwrd b){
	int  e1, e2, etemp;
	dwrd m1, m2, mtemp, nh, nl;
	dwrd result;
	
	e1=(a>>23)&0xFF; //得到阶码
	m1=(a&0x7FFFFF)|0x800000;  //得到尾数（整数部分1已经添上去了的那种） 
	printf("操作数1 -> 阶码：%u -- 尾数：%lX\n", e1, m1); 
	e2=(b>>23)&0xFF;
	m2=(b&0x7FFFFF)|0x800000;
	printf("操作数2 -> 阶码：%u -- 尾数：%lX\n", e2, m2);
	
	if(a==0||b==0)  //其中一个为0则结果一定是0 
		return a;  
	if((e1==255)&&(m1!=0)) //一个数为非数即结果为非数 
		return a;
	if((e2==255)&&(m2!=0))
		return b;
	if((e1==255)&&(m1==0)) //一个数为无穷大即结果为无穷大 
		return a;
	if((e2==255)&&(m2==0))
		return b;

	e1=(e1+e2-127); 
	nh=nl=0;
	while(m2!=0){
		nl>>=1; 
		if(nh&1)
			nl|=0x80000000;
		nh>>=1; 
		if(m2&1)
			nh+=m1;
		m2>>=1;
	}
	nh+=(nl>>31); //四舍五入
	m1=nh;

	while(m1&0xFF000000){m1>>=1; e1++;} //如果相乘后结果不是标准形式，进行调整 
	while(m1&0x00800000==0){m1<<=1; e1--;} //如果相乘后结果不是标准形式，进行调整
	result=((long)e1<<23)|(m1&0x7FFFFF)|((a^b)&0x80000000);
			
	return result;
}


dwrd fdiv(dwrd a, dwrd b){
	int  e1, e2, etemp;
	dwrd m1, m2, mtemp, I, J, j1, k, n;
	dwrd result;
	
	e1=(a>>23)&0xFF; //得到阶码
	m1=(a&0x7FFFFF)|0x800000;  //得到尾数（整数部分1已经添上去了的那种） 
	printf("操作数1 -> 阶码：%u -- 尾数：%lX\n", e1, m1); 
	e2=(b>>23)&0xFF;
	m2=(b&0x7FFFFF)|0x800000;
	printf("操作数2 -> 阶码：%u -- 尾数：%lX\n", e2, m2);
	
	if((e1==255)&&(m1!=0)) //一个数为非数即结果为非数 
		return a;
	if((e2==255)&&(m2!=0))
		return b;
	if(b==0)  //除数为0则结果为非数 
		return NaN;  
	if(a==0)  //被除数为0则结果为0
		return a; 
	if(((e1==255)&&(m1==0))||((e2==255)&&(m2==0))) //被除数、除数均为无穷大，结果为非数
	 	return NaN;  
	if((e1==255)&&(m1==0)) //被除数为无穷大即结果为无穷大 
		return a;

    e1 = e1 - e2 + 127;
	m1 = mmdiv(m1, m2);
	
	while(m1&0xFF000000){m1>>=1; e1++;} //如果相除后结果不是标准形式，进行调整 
	while(m1&0x00800000==0){m1<<=1; e1--;} //如果相除后结果不是标准形式，进行调整
	result=((long)e1<<23)|(m1&0x7FFFFF)|((a^b)&0x80000000);
			
	return result;
}

word64 mmdiv(word64 a,dwrd b){
    dwrd m,n,k; 
    word64 I,J,j1;

    j1=~J+1;
    n=1;
    for(k=0;k<32;k++){
        I=I<<1;
        if(n==1){
            I=I+(j1<<32);
        }
        else{
            I=I+(J<<32);
        }
        if(I&0x80000000)
            n=0;
        else
            n=1;
        I=I|n;
    }
    m=I&0x0000FFFF;  
    
    return m;
}
