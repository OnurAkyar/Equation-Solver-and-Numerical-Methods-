#include<stdio.h>
#include<stdlib.h>  
#include<math.h>
#include<string.h>
#include<ctype.h>

#define MAX_SIZE 100

// Matematiksel sabitler
#define pi 3.14159265
#define e  2.71828182

typedef struct{
    char *operator_stack;
    int capacity;
    int top;
}OPERATOR; // OPERATOR *stack tanımlı 

typedef struct{
    char *postfix;
    int top;
    int capacity;
}POSTFIX; // POSTFIX *pf tanımlı

typedef struct{
    double *postfix_cikti;
    int top;
    int capacity;
}POSTFIX_CIKTI;     //POSTFIX_CIKTI *pf_cikti tanımlı

//pars etme fonksiyonları
OPERATOR* init_stack();
POSTFIX* init_postfix();
POSTFIX_CIKTI* init_postfix_cikti();
void string_pars(char *denklem, OPERATOR *stack, POSTFIX *pf);
void pushtoPostfix(char *str, int str_leng, POSTFIX *pf);
void pushtoStack(char op, OPERATOR *stack);
void poptoPostfix(char op, OPERATOR *stack, POSTFIX *pf);
int oncelik(char op);
int sagdan_oncelik(char op);
int math_str(char *denklem, int i, char *buffer_str, OPERATOR *stack, POSTFIX *pf);
double result(POSTFIX *pf,POSTFIX_CIKTI *pf_cikti,double x);
void calculate_one_argument(char operator, POSTFIX_CIKTI *pf_cıktı);
void calculate_two_arguments(char operator, POSTFIX_CIKTI *pf_cıktı);

//yardımcı fonksiyonlar
void bellek_temizle(OPERATOR *stack, POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, char *denklem) ;
void freeMatrix(double **matrix, int N) ;
void printMatrix(double **matrix, int N) ;
double determinant(double **matrix, int N) ;
double DivMerkezi(POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, double h, double x);
double DivIleri(POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, double h, double x);
double DivGeri(POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, double h, double x);
void EnBuyukKosegen(double **matrix, double *results, int n);

// yöntemler
void Bisection();
void RegulaFalsi();
void NewtonRaphson();
void Matrix_Inverse();
void Cholesky();
void GaussSeidel();
void SayısalTurev();
void Simpson();
void Trapez();
void GregoryNewton();


int main(){
    int menu;

    while (1) {
        // Menü seçeneklerini yazdır
        printf("\n-----MENU-----");
        printf("\nGirmek istediginiz islemi secin\n");
        printf("Quit: 0\n");
        printf("Bisection: 1\n");
        printf("Regula-falsi: 2\n");
        printf("Newton-Raphson: 3\n");
        printf("NxNlik bir matrisin tersi: 4\n");
        printf("Cholesky: 5\n");
        printf("Gauss seidal: 6\n");
        printf("Sayisal turev: 7\n");
        printf("Simpson: 8\n");
        printf("Trapez: 9\n");
        printf("Degisken donusumsuz Gregory Newton enterpolasyonu: 10\n");

        printf("\nSeciminiz:");
        scanf("%d",&menu);
        switch (menu)
        {
        case 0:
             exit(1);
             break;
        case 1:
            Bisection();
            break;
        case 2:
            RegulaFalsi();
            break;
        case 3:
            NewtonRaphson();
            break;
        case 4:
            Matrix_Inverse();
            break;
        case 5:
            Cholesky();
            break;
        case 6:
            GaussSeidel();
            break;
        case 7:
            SayısalTurev();
            break;
        case 8:
            Simpson();
            break;
        case 9:
            Trapez();
            break;
        case 10:
            GregoryNewton();
            break;
        }
    }
    return 0;
}

OPERATOR* init_stack() {     //stack yığını için yer oluşturma
    OPERATOR *stack = (OPERATOR*)malloc(sizeof(OPERATOR));
    stack->capacity = 10;
    stack->top = -1;
    stack->operator_stack = (char*)calloc(stack->capacity,sizeof(char));
    return stack;
}


POSTFIX* init_postfix() {    //postfix yığını için yer oluşturma
    POSTFIX *pf = (POSTFIX*)malloc(sizeof(POSTFIX));
    pf->capacity = 10;
    pf->top = -1;
    pf->postfix = (char*)calloc(pf->capacity,sizeof(char));
    return pf;
}

POSTFIX_CIKTI* init_postfix_cikti(){  // postfix çıktısı için yer oluşturma
    POSTFIX_CIKTI *pf_cikti=(POSTFIX_CIKTI*)malloc(sizeof(POSTFIX_CIKTI));
    pf_cikti->capacity=10;
    pf_cikti->top=-1;
    pf_cikti->postfix_cikti=(double*)calloc(pf_cikti->capacity,sizeof(double));
    return pf_cikti;
}



void string_pars(char *denklem, OPERATOR *stack, POSTFIX *pf) {   //verilen denklemi pars ederek postfix ve stacke gönderme
    int i = 0, j = 0, sayi_var = 0, k=0;
    char *buffer_str = (char *)calloc(MAX_SIZE, sizeof(char));
    if (buffer_str == NULL) {
        printf("Yer açılamadı.\n");
        return;
    }
    
    for (i = 0; denklem[i] != '\0'; i++) {    //sayıları ayırma işlemi
        if ((denklem[i] >= '0' && denklem[i] <= '9') || denklem[i] == '.') {
            buffer_str[j++] = denklem[i];
            sayi_var = 1;
        } else {
            // "x", "e" veya "pi" ifadesini kontrol et
            if (denklem[i] == 'x' || denklem[i] == 'e') {
                buffer_str[j++] = denklem[i];
                sayi_var = 1;
                i++;
            } else if (denklem[i] == 'p' && denklem[i + 1] == 'i') {
                buffer_str[j++] = 'p';
                buffer_str[j++] = 'i';
                sayi_var = 1;
                i += 2;
            }
    
            if (sayi_var==1) {
                pushtoPostfix(buffer_str, j,pf);
    
                // Buffer'ı temizle
                for (k = 0; k < j; k++) {
                    buffer_str[k] = 0;
                }
                j = 0;
                sayi_var = 0;
            }
            // Fonksiyon veya operatör varsa işle
            if(denklem[i]!='\0'){ //en son gelen denklem[i]'nin stacke girip stacki sıfırlamaması için kontrol
                i = math_str(denklem, i, buffer_str,stack,pf);
            }
        }
    }
    
     // Denklem sonunda hâlâ sayı varsa onu da push et
    if (sayi_var) {
        pushtoPostfix(buffer_str, j,pf);
    }

    while (stack->top >= 0) {    //stackde kalan tüm operatörleri postfixe push et
        char top = stack->operator_stack[stack->top--];
        pushtoPostfix(&top, 1, pf);
    }    

    free(buffer_str);
}



    int math_str(char *denklem, int i, char *buffer_str, OPERATOR *stack, POSTFIX *pf) {
        int j=0,k=0;
    
        // Trigonometrik ve logaritmik fonksiyon adı varsa topla
        if (isalpha(denklem[i])) {
            while (isalpha(denklem[i])) {
                buffer_str[j++] = denklem[i++];
            }
            i--;  //fazladan artışı dengeler
    
            // Fonksiyon adlarını pushtoStack ile stack'e ekle
            if (strcmp(buffer_str, "cos") == 0) {
                poptoPostfix('c', stack, pf);
                pushtoStack('c', stack);
            } else if (strcmp(buffer_str, "sin") == 0) {
                poptoPostfix('s', stack, pf);
                pushtoStack('s', stack);
            } else if (strcmp(buffer_str, "tan") == 0) {
                poptoPostfix('t', stack, pf);
                pushtoStack('t', stack);
            } else if (strcmp(buffer_str, "cot") == 0) {
                poptoPostfix('o', stack, pf);
                pushtoStack('o', stack);
            } else if (strcmp(buffer_str, "arccos") == 0) {
                poptoPostfix('C', stack, pf);
                pushtoStack('C', stack);
            } else if (strcmp(buffer_str, "arcsin") == 0) {
                poptoPostfix('S', stack, pf);
                pushtoStack('S', stack);
            } else if (strcmp(buffer_str, "arctan") == 0) {
                poptoPostfix('T', stack, pf);
                pushtoStack('T', stack);
            } else if (strcmp(buffer_str, "arccot") == 0) {
                poptoPostfix('O', stack, pf);
                pushtoStack('O', stack);
            } else if (strcmp(buffer_str, "log") == 0) {
                poptoPostfix('l', stack, pf);
                pushtoStack('l', stack);
            }
    
        } else if (strchr("+-/*^(),", denklem[i])) {

            if((denklem[i]=='-' && i==0) || (denklem[i]=='(' && denklem[i+1]=='-')){   //Negatif sayı kontrolü
                buffer_str[0]='0';
                pushtoPostfix(buffer_str,1,pf);
            }

            if (denklem[i] == ',') { // virgül postfixe gönderilmemeli
                return i;
            }

            if (denklem[i] == ')') {
                while (stack->top >= 0 && stack->operator_stack[stack->top] != '(') {
                    buffer_str[0] = stack->operator_stack[stack->top--];
                    pushtoPostfix(buffer_str, 1, pf);
                }

                if (stack->top >= 0 && stack->operator_stack[stack->top] == '(')
                stack->top--; // '(' atla
            } else {
                if (denklem[i] != '('){
                    poptoPostfix(denklem[i], stack, pf);
                }
                pushtoStack(denklem[i], stack);
            }
        }

        for(k=0;k<j;k++){
            buffer_str[k]=0;
        }
        return i;
    }


    int oncelik(char op) {   //oncelik belirleme
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        if (op == '^') return 3;
        if (strchr("sctolCSTO", op)) return 4; // Fonksiyonlar en yüksek öncelikte
        return 0;
    }
    
    
    int sagdan_oncelik(char op) {  // ^'ın sağdan önceliği
        return (op == '^');
    }


    void poptoPostfix(char op, OPERATOR *stack, POSTFIX *pf) {
        int devam = 1,p_op,p_top;
        char top_value;
        while (stack->top >= 0 && devam==1) {
            top_value = stack->operator_stack[stack->top];
    
            if (top_value == '(') {
                devam = 0;  // '(' ile karşılaşınca dur
            } else {
                p_top = oncelik(top_value);  // yığının tepesindeki operatörün önceliği
                p_op  = oncelik(op);   // gelen yeni operatörün önceliği
    
                if (p_top > p_op || (p_top == p_op && !sagdan_oncelik(op))) {
                    // Eğer top öncelikli veya eşit öncelikli ve op soldan bağlıysa → pop et
                    pushtoPostfix(&top_value, 1, pf);  // postfix'e at
                    stack->top--;                // yığından düşür
                } else {
                    devam = 0;  // artık pop edilmemeli, dur
                }
            }
        }
    }


void pushtoPostfix(char *str, int str_leng, POSTFIX *pf) {      //string sayıları postfixe push etme
    int i;
    for(i = 0; i < str_leng; i++) {
        if (pf->top + 1 >= pf->capacity) {
            pf->capacity *= 2;
            pf->postfix = (char *)realloc(pf->postfix, pf->capacity * sizeof(char));
            if (pf->postfix==NULL) {
                printf("Postfix için bellek yetersiz!\n");
                exit(EXIT_FAILURE);
            }
        }
        pf->postfix[++pf->top] = str[i];
    }
    // Araya boşluk koyarak operandları ayırma
    pf->postfix[++pf->top] = ' ';
}


void pushtoStack(char op, OPERATOR *stack) {      //operatörleri stacke push etme
    if (stack->top + 1 >= stack->capacity) {
        stack->capacity *= 2;
        stack->operator_stack = (char *)realloc(stack->operator_stack, stack->capacity * sizeof(char));
        if (stack->operator_stack==NULL) {
            printf("Operatör yığını için bellek yetersiz!\n");
            exit(EXIT_FAILURE);
        }
    }
    stack->operator_stack[++stack->top] = op;
}


///// POSTFIX IFADENIN SONUCUNU BULMA

double result(POSTFIX *pf,POSTFIX_CIKTI *pf_cikti,double x){    //postfix_cikti yığınında hesaplama işlemlerinin yapılması
    int i=0,sayi_var=0,j=0;
    double number=0;
    char operator,buffer[50];
    for(i=0;pf->postfix[i]!='\0';i++){    //sayıyı oluşturma işlemi
        if((pf->postfix[i]>='0' && pf->postfix[i]<='9') || pf->postfix[i] == '.'){ 
            buffer[j++]=pf->postfix[i];
            sayi_var=1;
        }
        else {
            if(pf->postfix[i] == ' ' && sayi_var==1){
                if (i > 0 && isdigit(pf->postfix[i-1])){
                    if(pf_cikti->top+1 == pf_cikti->capacity){
                        pf_cikti->capacity *=2;
                        pf_cikti->postfix_cikti=(double*)realloc(pf_cikti->postfix_cikti, pf_cikti->capacity*sizeof(double));
                        if (pf_cikti->postfix_cikti==NULL) {
                            printf("Postfix için bellek yetersiz!\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                    buffer[j] = '\0';   //stringin sonuna null ifade konur
                    number = strtod(buffer, NULL);    // string double'a çevrilir
                    pf_cikti->postfix_cikti[++pf_cikti->top] = number;
                    number = 0;
                    sayi_var=0;
                    j=0;
                }    
            }else if(strchr("+-/*^l", pf->postfix[i])){   //operatör geldiğinde postfix_ciktidan sayı çekip hesaplama işlemi
                operator=pf->postfix[i];
                calculate_two_arguments(operator,pf_cikti);

            }else if (strchr("sctoCSTO", pf->postfix[i])){
                    operator=pf->postfix[i];
                    calculate_one_argument(operator,pf_cikti);

                }else if(pf->postfix[i]=='x'){
                    pf_cikti->postfix_cikti[++pf_cikti->top]=x;

                }else if(pf->postfix[i]=='e'){
                    pf_cikti->postfix_cikti[++pf_cikti->top]=e;
                }else if(pf->postfix[i]=='p' && pf->postfix[i+1]=='i'){
                    pf_cikti->postfix_cikti[++pf_cikti->top]=pi;
                    i++;
                }
        }
    }
    number=pf_cikti->postfix_cikti[0];
    pf_cikti->postfix_cikti[0]=0;  // döngülerde result fonksiyonunu birden fazla kez kullanılacağı için değerleri düzenleme
    pf_cikti->top=-1;
    pf_cikti->capacity=10;
    return number;
}   


void calculate_two_arguments(char operator, POSTFIX_CIKTI *pf_cikti){     //hesaplama yapması için 2 adet sayı gereken işlemler
    double argument1,argument2; //double argument1; // [3 5 8] gibi bir dizide 8 sayısı
                                //double argument2; // [3 5 8] gibi bir dizide 5 sayısı    ->  ^ için 5 taban 8 üsdür yani argument2 ^ argument1

    argument1=pf_cikti->postfix_cikti[pf_cikti->top];   //yığından sayıların çekilip işlenmesi
    argument2=pf_cikti->postfix_cikti[--pf_cikti->top];

    if(operator=='+'){
        pf_cikti->postfix_cikti[pf_cikti->top]=argument2 + argument1;
    }
    if(operator=='-'){
        pf_cikti->postfix_cikti[pf_cikti->top]=argument2 - argument1;
    }
    if(operator=='*'){
        pf_cikti->postfix_cikti[pf_cikti->top]=argument2 * argument1;
    }
    if(operator=='/'){
        if (argument1 == 0.0) {
            printf("HATA: 0'a bölme hatası!\n");
            exit(EXIT_FAILURE);
        }        
        pf_cikti->postfix_cikti[pf_cikti->top]=argument2 / argument1;
    }
    if(operator=='l'){
    if (argument1 <= 0.0) {
        printf("HATA: logaritma argümanı (log_b(a)) için a > 0 olmalıdır!\n");
        exit(EXIT_FAILURE);
    }
    if (argument2 <= 0.0 || argument2 == 1.0) {
        printf("HATA: logaritmanın tabanı (log_b(a)) için b > 0 ve b ≠ 1 olmalıdır!\n");
        exit(EXIT_FAILURE);
    }
    pf_cikti->postfix_cikti[pf_cikti->top]=log(argument1) / log(argument2);
}
    if(operator=='^'){
        pf_cikti->postfix_cikti[pf_cikti->top]=pow(argument2,argument1);
    }
}


void calculate_one_argument(char operator, POSTFIX_CIKTI *pf_cikti){ //hesaplama yapması için 1 adet sayı gereken işlemler
    double argument1;

    argument1=pf_cikti->postfix_cikti[pf_cikti->top];
    
    if(operator=='c'){
        pf_cikti->postfix_cikti[pf_cikti->top]= cos(argument1);
    }
    if(operator=='s'){
        pf_cikti->postfix_cikti[pf_cikti->top]= sin(argument1);
    }
    if(operator=='t'){
        pf_cikti->postfix_cikti[pf_cikti->top]= tan(argument1);
    }
    if(operator=='o'){
        pf_cikti->postfix_cikti[pf_cikti->top]= 1/tan(argument1);
    }
    if(operator=='C'){ // acos
        if (argument1 < -1.0 || argument1 > 1.0) {
            printf("HATA: acos argümanı [-1, 1] aralığında olmalıdır!\n");
            exit(EXIT_FAILURE);
        }
        pf_cikti->postfix_cikti[pf_cikti->top]= acos(argument1); 
    }
    if(operator=='S'){ // asin
        if (argument1 < -1.0 || argument1 > 1.0) {
            printf("HATA: asin argümanı [-1, 1] aralığında olmalıdır!\n");
            exit(EXIT_FAILURE);
        }
        pf_cikti->postfix_cikti[pf_cikti->top]= asin(argument1); 
    }
    if(operator=='T'){ // atan
        pf_cikti->postfix_cikti[pf_cikti->top]= atan(argument1); 
    }
    if(operator=='O'){ // acot
        pf_cikti->postfix_cikti[pf_cikti->top]= ((pi/2.0)-atan(argument1));
    }
} 

/////////////////  KULLANILACAK YARDIMCI FONKSİYONLAR

//yıgınları ve denklemin belleğini serbest bırakma fonksiyonu
void bellek_temizle(OPERATOR *stack, POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, char *denklem) {
    if (stack != NULL) {
        free(stack->operator_stack);
        free(stack);
    }

    if (pf != NULL) {
        free(pf->postfix);
        free(pf);
    }

    if (pf_cikti != NULL) {
        free(pf_cikti->postfix_cikti);
        free(pf_cikti);
    }

    if (denklem != NULL) {
        free(denklem);
    }
}

// Matris belleğini serbest bırakma fonksiyonu
void freeMatrix(double **matrix, int N) {
    int i;
    for (i=0;i<N;i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Matrisi yazdırma fonksiyonu
void printMatrix(double **matrix, int N) 
{
    int i,j;
    for (i=0;i<N;i++) 
    {
        for (j=0;j<N;j++) 
        {
            printf("%lf  ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Matrisin determinantını hesaplayan fonksiyon
double determinant(double **matrix, int N) {
    double det = 1.0,factor,*temp;
    int i,k,j,swapFound=0;
    // Satır satır ilerleyerek matrisin üst üçgensel hale getirilmesi
    for (i=0;i<N;i++) {
        // Eğer köşegen eleman sıfırsa, sıfır olmayan bir elemanla yer değiştir
        if (matrix[i][i] == 0.0) {
            swapFound = 0;
            for (k=i+1;k<N && !swapFound;k++) {
                if (matrix[k][i] != 0.0) {
                    // Satırları değiştir ve determinantı negatifle
                    temp = matrix[i];
                    matrix[i] = matrix[k];
                    matrix[k] = temp;
                    det *= -1;
                    swapFound = 1;
                }
            }
            // Eğer sıfırdan farklı bir eleman bulunamazsa, determinant sıfırdır
            if (!swapFound) 
            {
                return 0.0; // Matris tekildir
            }
        }

        // İleri eleme yöntemi ile matrisi üst üçgensel hale getir
        for (k=i+1;k<N;k++){
            factor = matrix[k][i] / matrix[i][i];
            for (j=i;j<N;j++) {
                matrix[k][j] -= factor * matrix[i][j];
            }
        }
        // Determinant değerini güncelle
        det *= matrix[i][i];
    }
    return det;
}

double DivMerkezi(POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, double h, double x){  //merkezi farklar için türev fonksiyonu
    double ileri,geri;
    ileri=result(pf,pf_cikti,x+h);   //f(x+h)'ı hesaplar
    geri=result(pf,pf_cikti,x-h);    //f(x-h)'ı hesaplar
    return (ileri-geri)/(2*h);
}


double DivIleri(POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, double h, double x){  //ileri farklar için türev fonksiyonu
    double ileri,geri;
    ileri=result(pf,pf_cikti,x+h);   //f(x+h)'ı hesaplar
    geri=result(pf,pf_cikti,x);     //f(x)'ı hesaplar
    return (ileri-geri)/h;
}

double DivGeri(POSTFIX *pf, POSTFIX_CIKTI *pf_cikti, double h, double x){  //geri farklar için türev fonksiyonu
    double ileri,geri;
    ileri=result(pf,pf_cikti,x);   //f(x)'ı hesaplar
    geri=result(pf,pf_cikti,x-h);     //f(x-h)'ı hesaplar
    return (ileri-geri)/h;
}

// Matrisin köşegeninde en büyük çarpanı elde eden ve gerekirse satır değişimi yapan fonksiyon
void EnBuyukKosegen(double **matrix, double *results, int n) {
    int i,j,maxRow;
    double *tempRow, tempResult;
    for (i=0;i<n;i++) {
        maxRow = i;
        double maxElement = fabs(matrix[i][i]);

        for (j=i+1;j<n;j++) {
            if (fabs(matrix[j][i]) > maxElement) {
                maxElement = fabs(matrix[j][i]);
                maxRow = j;
            }
        }
        if (maxRow != i) {       
            tempRow = matrix[i];               // Eğer bulunan en büyük elemanın indisinde değişiklik yapıldıysa,
            matrix[i] = matrix[maxRow];          // bu iki satırı yer değiştirme
            matrix[maxRow] = tempRow;     
            
            tempResult = results[i];           //sonuç vektöründeki satırları değiştirme
            results[i] = results[maxRow];
            results[maxRow] = tempResult;
        }
    }
}


/////////////////// YONTEMLER

void Bisection(){
    double x, hata_tolerans, left, right, left_value, right_value, hata, new, new_value, new_old;
    int i = 0, j = 0, tmp = 0;
    char *denklem = (char *)calloc(MAX_SIZE, sizeof(char));
    OPERATOR *stack = init_stack();
    POSTFIX *pf = init_postfix();
    POSTFIX_CIKTI *pf_cikti = init_postfix_cikti();

    printf("kökünü bulmak istediğiniz denklemi giriniz: ");
    scanf("%s", denklem);

    while (denklem[i] != '\0' && tmp == 0) {   // denklemde x var mı kontrol yapılır
        if (denklem[i] == 'x'){
            tmp = 1;
        } 
        i++;
    }
    if (tmp == 0) {
        printf("girdiginiz ifade x icermemektedir.\n");
        exit(0);
    }

    string_pars(denklem, stack, pf);

    printf("hata payini giriniz: ");
    scanf("%lf", &hata_tolerans);

    printf("Araligin baslangicini giriniz: ");
    scanf("%lf", &left);
    x = left;
    left_value = result(pf, pf_cikti, x);

    printf("Araligin bitisini giriniz: ");
    scanf("%lf", &right);
    x = right;
    right_value = result(pf, pf_cikti, x);

    if (right_value * left_value > 0) {   // kök var mı kontrol edilir
        printf("kok yoktur.\n");
        exit(0);
    } else if (right_value * left_value == 0) {
        if (right_value == 0) {
            printf("kok = %lf'dir\n", right);
            exit(0);
        } else if (left_value == 0) {
            printf("kok = %lf'dir\n", left);
            exit(0);
        }
    }

    hata = fabs(right - left);
    new_old = left;

    // TABLO BAŞLIĞI
    printf("\n| Iteration |     a     |     b     |     c     |   f(c)    |  |Hata|  |\n");
    printf("--------------------------------------------------------------------------\n");

    while (hata_tolerans < hata) {    //hata, hata_toleranstan az olana kadar döngüde işlemler devam eder
        new = (right + left) / 2.0;
        new_value = result(pf, pf_cikti, new);

        hata = fabs(new - new_old);

         if (j == 0) {
            printf("| %9d | %9.6lf | %9.6lf | %9.6lf | %9.6lf | %13.6lf |\n", j + 1, left, right, new, new_value, hata); //tablo oluşturma
        } else {
            printf("| %9d | %9.6lf | %9.6lf | %9.6lf | %9.6lf | %13.6lf |\n", j + 1, left, right, new, new_value, hata);
        }

        if (new_value * left_value > 0) {
            left = new;
            left_value = new_value;
        } else if (new_value * right_value > 0) {
            right = new;
            right_value = new_value;
        }

        new_old = new;
        j++;
    }
    printf("\n");
    // SONUÇ BİLGİLERİ
    printf("%d. iterasyonda yaklasik koku = %lf --- hata orani = %lf\n",j,new,hata);
    bellek_temizle(stack, pf, pf_cikti, denklem);
}



void RegulaFalsi(){
    double x, hata_tolerans, hata, left, right, left_value, right_value, new, new_value, new_old;
    int i=0, j=1, tmp=0;
    char *denklem = (char *)calloc(MAX_SIZE, sizeof(char));
    OPERATOR *stack = init_stack();
    POSTFIX *pf = init_postfix();
    POSTFIX_CIKTI *pf_cikti = init_postfix_cikti();

    printf("kökünü bulmak istediğiniz denklemi giriniz: ");
    scanf("%s", denklem);

    while (denklem[i] != '\0' && tmp == 0) {  // denklemde x var mı kontrol yapılır
        if (denklem[i] == 'x') {
        tmp = 1;
        }
        i++;
    }
    if (tmp == 0) {
        printf("girdiginiz ifade x icermemektedir.\n");
        exit(0);
    }

    string_pars(denklem, stack, pf);

    printf("hata payini giriniz: ");
    scanf("%lf", &hata_tolerans);

    printf("Araligin baslangicini giriniz: ");
    scanf("%lf", &left);
    left_value = result(pf, pf_cikti, left);

    printf("Araligin bitisini giriniz: ");
    scanf("%lf", &right);
    right_value = result(pf, pf_cikti, right);

    if (right_value * left_value > 0) {     // kök var mı kontrol edilir
        printf("kok yoktur.\n");
        exit(0);
    } else if (right_value * left_value == 0) {
        if (right_value == 0) {
            printf("kok = %lf'dir\n", right);
            exit(0);
        } else if (left_value == 0) {
            printf("kok = %lf'dir\n", left);
            exit(0);
        }
    }

    // İlk new ve hata hesaplanır
    new = ((right * left_value) - (left * right_value)) / (left_value - right_value);
    new_value = result(pf, pf_cikti, new);
    new_old = new;  // ilk değer
    if(new_value<0){      //while döngüsüne girip girmemek için ilk hata değeri hesaplanır
        hata = fabs(new-left);
    }else{
        hata = fabs(new-right);
    }

    // TABLO BAŞLIĞI
    printf("\n| Iteration |     a     |     b     |     c     |   f(c)    |   |Hata|   |\n");
    printf("---------------------------------------------------------------------------\n");

    // İlk iterasyon
    printf("| %9d | %9.6lf | %9.6lf | %9.6lf | %9.6lf |   %9.6lf   |\n", j, left, right, new, new_value, hata);
    j++;

    while (hata > hata_tolerans) {     //hata, hata_toleranstan az olana kadar döngüde işlemler devam eder
        if (new_value * left_value > 0) {
            left = new;
            left_value = new_value;
        } else {
            right = new;
            right_value = new_value;
        }

        new = ((right * left_value) - (left * right_value)) / (left_value - right_value);
        new_value = result(pf, pf_cikti, new);
        hata = fabs(new - new_old);

        printf("| %9d | %9.6lf | %9.6lf | %9.6lf | %9.6lf | %11.6lf |\n", j, left, right, new, new_value, hata);

        new_old = new;
        j++;
    }

    printf("\n%d. iterasyonda yaklasik koku = %lf --- hata orani = %lf\n", j - 1, new, hata);
    bellek_temizle(stack, pf, pf_cikti, denklem);
}



void NewtonRaphson(){
    double x, new, x_value, x_divValue, hata_tolerans, hata, h = 0.0001;
    int j = 1, i = 0, tmp = 0;
    char *denklem = (char *)calloc(MAX_SIZE, sizeof(char));
    OPERATOR *stack = init_stack();
    POSTFIX *pf = init_postfix();
    POSTFIX_CIKTI *pf_cikti = init_postfix_cikti();

    printf("kökünü bulmak istediğiniz denklemi giriniz: ");
    scanf("%s", denklem);

    while (denklem[i] != '\0' && tmp == 0) {
        if (denklem[i] == 'x') {
            tmp = 1;
        }
        i++;
    }
    if (tmp == 0) {
        printf("girdiginiz ifade x icermemektedir.\n");
        exit(0);
    }

    string_pars(denklem, stack, pf);

    printf("hata payini giriniz: ");
    scanf("%lf", &hata_tolerans);

    printf("baslangic degerini giriniz: ");
    scanf("%lf", &x);

    // TABLO BAŞLIĞI (fotoğraftaki gibi + hata sütunu)
    printf("\n%-5s     %-10s     %-10s     %-10s     %-10s     %-10s\n", "n", "x₀", "f(x₀)", "f'(x₀)", "x₁", "hata");
    printf("--------------------------------------------------------------------------\n");

    x_value = result(pf, pf_cikti, x);
    x_divValue = DivMerkezi(pf, pf_cikti, h, x);    // türevini hesaplama

    if (x_divValue == 0) {
        printf("Türev degeri sıfırdır. Newton-Raphson yöntemi uygulanamaz.\n");
        exit(0);
    }

    new = x - (x_value / x_divValue);
    hata = fabs(new - x);

    // İlk iterasyon çıktısı
    printf("%-5d   %-10.6lf   %-10.6lf   %-10.6lf   %-10.6lf   %-10.6lf\n", j, x, x_value, x_divValue, new, hata);

    x = new;
    j++;

    while (hata > hata_tolerans) {       //hata, hata_toleranstan az olana kadar döngüde işlemler devam eder
        x_value = result(pf, pf_cikti, x);
        x_divValue = DivMerkezi(pf, pf_cikti, h, x);

        if (x_divValue == 0) {
            printf("Türev degeri sıfırdır. Newton-Raphson yöntemi uygulanamaz.\n");
            exit(0);
        }
        new = x - (x_value / x_divValue);
        hata = fabs(new - x);

        printf("%-5d   %-10.6lf   %-10.6lf   %-10.6lf   %-10.6lf   %-10.6lf\n", j, x, x_value, x_divValue, new, hata);

        x = new;
        j++;
    }
    printf("\n%d. iterasyonda yaklasik koku = %.6lf --- hata orani = %.6lf\n", j - 1, x, hata);
    bellek_temizle(stack, pf, pf_cikti, denklem);
}



void Matrix_Inverse(){
    double x, *temp, factor, det;
    int i=0, j=1, tmp=0, k=0, N, swapFound=0;
    printf("Kare matrisin boyutunu giriniz(N):\n");
    scanf("%d",&N);

    double **matrix=(double**)malloc(N*sizeof(double*));
    double **inv_matrix=(double**)malloc(N*sizeof(double*));
    double **copy_matrix=(double**)malloc(N*sizeof(double*));
    for(i=0;i<N;i++){
        matrix[i]=(double*)malloc(N*sizeof(double));
        inv_matrix[i]=(double*)malloc(N*sizeof(double));
        copy_matrix[i]=(double*)malloc(N*sizeof(double));
        inv_matrix[i][i]=1.0;      // birim matris oluşturma
    }

    printf("matrisin elemanlarını giriniz:\n");
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            printf("[%d][%d] = ",i+1,j+1);
            scanf("%lf",&matrix[i][j]);
        }
    }

    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            copy_matrix[i][j]=matrix[i][j];
        }
    }

    det=determinant(copy_matrix,N);
    if(det==0){
        printf("matrisin tersi alınamaz.\n");
        exit(0);
    }

    //üst üçgen haline getirdik
    for (i=0;i<N;i++) {
        // Eğer köşegen eleman sıfırsa, sıfır olmayan bir elemanla yer değiştir
        if (matrix[i][i] == 0.0) {
            swapFound = 0;
            for (k=i+1;k<N && !swapFound;k++) {
                if (matrix[k][i] != 0.0) {
                    // Satırları değiştir
                    temp = matrix[i];
                    matrix[i] = matrix[k];
                    matrix[k] = temp;
                    temp = inv_matrix[i];
                    inv_matrix[i] = inv_matrix[k];
                    inv_matrix[k] = temp;
                    swapFound = 1;
                }
            }
           // Satır değişimi yapılamazsa
            if (!swapFound) {
                printf("Matris tekildir ve tersi yoktur.\n");
                // Bellekten serbest bırakma
                freeMatrix(matrix, N);
                freeMatrix(inv_matrix, N);
                exit(0); 
            }
        }

        // İleri eleme yöntemi ile matrisi üst üçgensel hale getir
        for (k=i+1;k<N;k++){
            factor = matrix[k][i] / matrix[i][i];
            for (j=0;j<N;j++) {
                matrix[k][j] -= factor * matrix[i][j];
                inv_matrix[k][j] -= factor * inv_matrix[i][j];
            }
        }
    }

    //üst üçgenseli birim matris haline getirme
    for (i=N-1;i>=0;i--) {
        for (j=0;j<N;j++) {
            inv_matrix[i][j] /= matrix[i][i];
        }
        matrix[i][i]/=matrix[i][i]; //köşegen elemanlarını 1 yapma
        
        for (k=i-1;k>=0;k--) {
            factor=matrix[k][i];
            for (j=0;j<N;j++) {
                matrix[k][j] -= factor * matrix[i][j];
                inv_matrix[k][j] -= factor * inv_matrix[i][j];
            }
        }
    }
    printf("matrisin tersi = ");
    printf("\n");
    printMatrix(inv_matrix,N);
    freeMatrix(matrix, N);
    freeMatrix(inv_matrix, N);
}



void Cholesky(){
    int i, j, k, n;
    double sum;

    printf("Matris boyutunu girin (n x n): ");
    scanf("%d", &n);

    // Matrisler ve vektörler için bellek ayırma
    double **A = (double **)malloc(n * sizeof(double *));   // Katsayılar matrisi [A] için yer açma
    double **L = (double **)malloc(n * sizeof(double *));   // Alt matris [L] için yer açma
    double **U = (double **)malloc(n * sizeof(double *));   // Üst matris [U] için yer açma
    double *C = (double *)malloc(n * sizeof(double));       // [L][U][X]=[C] için yer açma  
    double *X = (double *)malloc(n * sizeof(double)); 
    double *Y = (double *)malloc(n * sizeof(double));       // [U][X]=[Y] için yer açma

    for (i = 0; i < n; i++) {
    A[i] = (double *)malloc(n * sizeof(double));
    L[i] = (double *)malloc(n * sizeof(double));
    U[i] = (double *)malloc(n * sizeof(double));
    }

     // A Matrisinin elemanlarını kullanıcıdan alma
    printf("A matrisinin elemanlarini girin (Ax=C):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("A[%d][%d]: ", i+1,j+1);
            scanf("%lf", &A[i][j]);
        }
    }

    // C matrisinin elemanlarını kullanıcıdan alma
    printf("C vektörünün (sağ taraf) elemanlarini girin:\n");
    for (i = 0; i < n; i++) {
        printf("C[%d]: ", i + 1);
        scanf("%lf", &C[i]);
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            L[i][j] = 0.0;
            U[i][j] = 0.0;
        }
        U[i][i] = 1.0; 
    }

    for (i = 0; i < n; i++) { 
        for (j = 0; j <= i; j++) { 
            sum = 0.0;
            for (k = 0; k < j; k++) {
                sum += L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - sum;
        }

        for (j = i + 1; j < n; j++) { 
            sum = 0.0;
            for (k = 0; k < i; k++) {
                sum += L[i][k] * U[k][j];
            }
            if (L[i][i] == 0.0) {
                printf("Hata: L[%d][%d] (köşegen) sıfır olamaz!\n", i, i);
                exit(EXIT_FAILURE);
            }
            U[i][j] = (A[i][j] - sum) / L[i][i];
        }
    }

    for (i = 0; i < n; i++) {  // [L] * [Y] = [C] matrisini çözer
        sum = 0.0;
        for (j = 0; j < i; j++) { 
            sum += L[i][j] * Y[j];
        }
        if (L[i][i] == 0.0) {
            printf("Hata: L[%d][%d] (köşegen) sıfır. İleri yerine koyma başarısız.\n", i, i);
            exit(EXIT_FAILURE);
        }
        Y[i] = (C[i] - sum) / L[i][i];
    }

    for (i = n - 1; i >= 0; i--) {   // [U] * [X] = [Y] matrisini çözer ve X bilinmeyenleri bulunur.
        sum = 0.0;
        for (j = i + 1; j < n; j++) {
            sum += U[i][j] * X[j];
        }
        if (U[i][i] == 0.0) {
             printf("Hata: U[%d][%d] (köşegen) sıfır. Geri yerine koyma başarısız.\n", i, i);
             exit(EXIT_FAILURE);
        }
        X[i] = (Y[i] - sum) / U[i][i]; 
    }

    printf("\n");
    printf("[L][U][X] = [C]\n");
    printf("[Lower] matrisi:\n");
    printMatrix(L,n);
    printf("[Upper] matrisi:\n");
    printMatrix(U,n);
    printf("[U][X]=[Y] ara matrisi:\n");
    for(i=0;i<n;i++){
        printf("%lf\n",Y[i]);
    }

    printf("\nDenklem sisteminin çözümü:\n");
    for(i=0;i<n;i++){
        printf("X%d = %lf\n",i+1,X[i]);
    }

    // Belleği serbest bırak
    for (i = 0; i < n; i++) {
        free(A[i]);
        free(L[i]);
        free(U[i]);
    }
    free(A);
    free(L);
    free(U);
    free(C);
    free(Y);
    free(X);
}



void GaussSeidel(){
    int n, i, j, iterations;
    double det, maxDifference, tolerance, difference;
    char deg='a';

    printf("Degisken sayisini giriniz: ");
    scanf("%d",&n);

    double **matrix = (double **)calloc(n, sizeof(double *));
    for (i = 0; i < n; i++) {
        matrix[i] = (double *)calloc(n, sizeof(double)); // Katsayı matrisi
    }
    double *sonuc = (double *)calloc(n, sizeof(double)); // Sonuç matrisi


    //matrixin prototipinin yazdırılması ve kullanıcıdan değerlerin alınması
    for (i=0;i<n;i++){
        for (j=0;j<n;j++) {
            if (j == n-1) {
                printf("%c%d ", deg+j,i+1);
            } else {
                printf("%c%d + ",deg+j,i+1);
            }
        }
        printf("= %c%d\n",deg+j,i+1);
    }
    printf("Degiskenleri giriniz:\n");
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            printf("%c%d: ", deg+j,i+1);
            scanf("%lf", &matrix[i][j]);
        }
        printf("%c%d: ", deg+j,i+1);
        scanf("%lf", &sonuc[i]);
        printf("\n");
    }

    //determinant kontrol etme 0 ise çıkma
    double **matrixCopy = (double**)calloc(n, sizeof(double *));
    for (i=0;i<n;i++) {
        matrixCopy[i] = (double*)calloc(n, sizeof(double));
        for (j=0;j<n;j++) 
        {
            matrixCopy[i][j]=matrix[i][j];
        }
    }
    det=determinant(matrixCopy, n);
    if (det==0.0) {
        printf("Matris tekildir ve tersi alinamaz.\n");
        freeMatrix(matrix, n);
        freeMatrix(matrixCopy, n);
        exit(0);
    }
    freeMatrix(matrixCopy, n);
    EnBuyukKosegen(matrix,sonuc,n);

    // Başlangıç değerlerinin alınması ve hata payının girilmesi
    double *basDeg = (double *)calloc(n, sizeof(double));
    printf("Baslangic degerlerini giriniz:\n");
    for (i=0;i<n;i++) {
        printf("%c[%d]:",'a'+i,i+1);
        scanf("%lf", &basDeg[i]);
    }

    printf("Hata payini giriniz: ");
    scanf("%lf", &tolerance);

    printf("Iteration\t ");
    // Önceki iterasyondaki değerlerin saklanması
    double *oncekiX = (double *)calloc(n, sizeof(double));
    for (i=0;i<n;i++) {
        oncekiX[i] = basDeg[i];
        printf(" %c\t\t |Δ%c|\t\t ",'a'+i,'a'+i);
    }
    printf("\n");
    printf("  1\t\t");
    
    for(i=0;i<n;i++){
    printf("%.6lf\t    -\t        ",basDeg[i]);
    }
    printf("\n");

    iterations = 0;
    do{
        // Gauss-Seidel iterasyonunun yapılması
        for (i=0;i<n;i++) {
            double sum = sonuc[i];
            for (j=0;j<n;j++) 
            {
                if (i!=j) 
                {
                    sum-=matrix[i][j]*basDeg[j];
                }
            }
            basDeg[i]=sum/matrix[i][i];
        }

        // Yakınsama kontolü
        maxDifference=0;
        printf("  %d\t\t",iterations+2);
        for (i=0;i<n;i++){
            difference=fabs(basDeg[i]-oncekiX[i]);
            if (difference > maxDifference) 
            {
                maxDifference=difference;
            }
            oncekiX[i]=basDeg[i];  // önceki iterasyonun güncellenmesi
            printf("%.6lf\t",basDeg[i]);
            printf("%.6lf\t",difference);
        }
        printf("\n");
        iterations++;
    } while (maxDifference>tolerance);

    printf("Gauss-Seidel iterasyonu %d iterasyonda tamamlandi.\n", iterations+1);

    for (i=0;i<n;i++) {
        printf("%c[%d] = %lf\n",'a'+i,i+1,basDeg[i]);
    }
    freeMatrix(matrix,n);
    free(sonuc);
    free(basDeg);
}



void SayısalTurev(){
    int i=0, tmp=0;
    double x, h, merkezi_div, ileri_div, geri_div;

    char *denklem = (char *)calloc(MAX_SIZE, sizeof(char));
    OPERATOR *stack = init_stack();
    POSTFIX *pf = init_postfix();
    POSTFIX_CIKTI *pf_cikti = init_postfix_cikti();

    printf("türevini bulmak istediğiniz denklemi giriniz: ");
    scanf(" %s",denklem);

        //x var mı diye arar varsa x değerini sorar yoksa fonksiyondan çıkar
    while(denklem[i]!='\0' && tmp==0){
        if(denklem[i]=='x'){
            tmp=1;
        }
        i++;
    }
    if(tmp==0){
        printf("girdiginiz ifade x icermemektedir.\n");
        exit(0);
    }

    string_pars(denklem, stack, pf);

    printf("X değerini giriniz:");
    scanf("%lf",&x);

    printf("h değerini giriniz:");
    scanf("%lf",&h);

    ileri_div=DivIleri(pf,pf_cikti,h,x);    //ileri türev fonksiyonu
    printf("Ileri farklar icin turev:%lf\n",ileri_div);

    geri_div=DivGeri(pf,pf_cikti,h,x);     //geri türev fonksiyonu
    printf("Geri farklar icin turev:%lf\n",geri_div);

    merkezi_div=DivMerkezi(pf,pf_cikti,h,x); //merkezi türev fonksiyonu
    printf("Merkezi farklar icin turev:%lf\n",merkezi_div);

    bellek_temizle(stack,pf,pf_cikti,denklem);
}



void Simpson(){
    double left, right, left_value, right_value, new, sum = 0.0, h = 0.0;
    double step1, step2, step1_value, step2_value, a, b, x_i, f_xi;
    int i = 0, n;

    char *denklem = (char *)calloc(MAX_SIZE, sizeof(char));
    OPERATOR *stack = init_stack();
    POSTFIX *pf = init_postfix();
    POSTFIX_CIKTI *pf_cikti = init_postfix_cikti();

    printf("integralini bulmak istediğiniz denklemi giriniz: ");
    scanf("%s", denklem);

    string_pars(denklem, stack, pf);

    printf("Araligin baslangicini giriniz: ");
    scanf("%lf", &left);

    printf("Araligin bitisini giriniz: ");
    scanf("%lf", &right);

    printf("araligin kac parcadan olusacagini giriniz(n): ");
    scanf("%d", &n);

    // Simpson 1/3 kuralı
    if (n % 2 == 0) {       //simpson 1/3 kuralı için n değerinin çift olması kontrolü
        h = (right - left) / n;
        left_value = result(pf, pf_cikti, left);     // Y0 değeri
        right_value = result(pf, pf_cikti, right);   // Yn değeri
        sum = 0;

        for (i = 1; i < n; i++) {
            x_i = left + i * h;
            f_xi = result(pf, pf_cikti, x_i);

            if (i % 2 == 0)
                sum += 2 * f_xi;  // i çift ise 2*f(xi) uygulanacak
            else
                sum += 4 * f_xi;   //i tekse 4*f(xi+1) uygulanacak
        }

        sum = (h / 3.0) * (left_value + sum + right_value);      //simpson 1/3 kuralının uygulanması
        printf("Simpson 1/3 kuralina gore integral degeri = %lf\n", sum);
    } else {
        printf("n sayisi tektir. Simpson 1/3 yöntemi kullanılamaz.\n");
    }

    // Simpson 3/8 kuralı
    sum = 0;
    h = (right - left) / n;
    a = left;
    b = right;

    for (i = 0; i < n; i++) {
        left = a + i * h;         // integralin parçalara ayrılması
        right = left + h;
        step1 = left + (right - left) / 3.0;
        step2 = left + 2 * (right - left) / 3.0;
        left_value = result(pf, pf_cikti, left);
        step1_value = result(pf, pf_cikti, step1);
        step2_value = result(pf, pf_cikti, step2);
        right_value = result(pf, pf_cikti, right);

        sum += (right - left) * (left_value + 3 * step1_value + 3 * step2_value + right_value) / 8.0;     //simpson 3/8 kuralının uygulanması
    }
    printf("Simpson 3/8 kuralina gore integral degeri = %lf\n", sum);
    bellek_temizle(stack, pf, pf_cikti, denklem);
}




void Trapez(){
    double left, right, left_value, right_value, sum = 0.0, h = 0.0, x_i;
    int i = 0, n, tmp = 0;;
    char *denklem = (char *)calloc(MAX_SIZE,sizeof(char));
    OPERATOR *stack = init_stack();
    POSTFIX *pf = init_postfix();
    POSTFIX_CIKTI *pf_cikti = init_postfix_cikti();

    printf("integralini bulmak istediğiniz denklemi giriniz:");
    scanf("%s", denklem);
    
    //x var mı diye arar varsa x değerini sorar yoksa fonksiyondan çıkar
    while(denklem[i] != '\0' && tmp == 0){
        if(denklem[i] == 'x'){
            tmp = 1;
        }
        i++;
    }
    if(tmp == 0){
        printf("girdiginiz ifade x icermemektedir.\n");
        exit(0);
    }

    string_pars(denklem, stack, pf);

    printf("Araligin baslangicini giriniz: ");
    scanf("%lf", &left);

    printf("Araligin bitisini giriniz: ");
    scanf("%lf", &right);

    printf("araligin kac parcadan olusacagini giriniz(n): ");
    scanf("%d", &n);

    h = (right - left) / n;
    left_value = result(pf, pf_cikti, left);     // Y0 değeri
    right_value = result(pf, pf_cikti, right);  // Yn değeri

    // ara noktaların toplamı
    for(i = 1; i < n; i++) {
        x_i = left + i * h;
        sum += result(pf, pf_cikti, x_i);    // Y1+Y2+...+Yn-1 değerleri
    }
    sum = h * ((left_value + right_value) / 2.0 + sum);      //trapez kuralının uygulanması

    printf("Trapez yöntemine göre verilen denklemin yaklaşık integral degeri = %lf\n", sum);
    bellek_temizle(stack, pf, pf_cikti, denklem);
}



void GregoryNewton(){
    int N,i,j=1;
    double h, baslangic, value, **sonluFark, sonuc, tmp, polation;

    printf("x veya f(x) sayısını giriniz: ");
    scanf("%d",&N);

    double *x = (double*)malloc(N*sizeof(double));
    if(x==NULL){
        printf("yer açma hatası.\n");
        exit(0);
    }
    
    double *y = (double*)malloc(N*sizeof(double));
    if(y==NULL){
        printf("yer açma hatası.\n");
        exit(0);
    }
    
    printf("h değerini giriniz: ");
    scanf("%lf",&h);

    printf("X0 başlangıç noktasını giriniz: ");
    scanf("%lf",&baslangic);

    for(i=0;i<N;i++){
        x[i]=baslangic+i*h;
        printf("%d. noktanın f(x) değerini giriniz: ",i+1);
        scanf("%lf",&value);
        y[i]=value;
    }

    sonluFark=(double**)calloc(N,sizeof(double*));  //sonlu fark tablosu için yer açma
    for(i=0;i<N;i++){
        sonluFark[i]=(double*)calloc(N,sizeof(double));
    }

    for(i=0;i<N;i++){
        sonluFark[i][0]=y[i];
    }

    for(j=0;j<N-1;j++){   //sonlu fark tablosu oluşturma
        for(i=j;i<N-1;i++){
            sonluFark[i+1][j+1]=sonluFark[i+1][j]-sonluFark[i][j];
        }
    }

    printf("\n");
    printf("SONLU FARKLAR TABLOSU:\n");
    for(i=0;i<N;i++){       //sonlu fark tablosunu yazdırma
        for(j=0;j<=i;j++){
            printf("%.3lf   ",sonluFark[i][j]);
        }
        printf("\n");
    }

    printf("\nhangi değerde enterpolasyon yapacaksınız:");
    scanf("%lf",&polation);

    tmp=1.0;
    sonuc=y[0];

    for (i=1;i<N;i++){
        tmp *=(polation-x[i-1])/(i);   // (x - x0)(x - x1)...(x - xi-1) / (i!)
        sonuc += (tmp * sonluFark[i][i])/pow(h,i);  // Her yeni terim eklenir
    }

    printf("\nEnterpolasyon sonucu:%lf\n\n",sonuc);
    for(i=0;i<N;i++){
        free(sonluFark[i]);
    }
    free(sonluFark);
    free(x);
    free(y);
}
