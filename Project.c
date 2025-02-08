#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
/*
@brief Bu fonksiyonda 1'den N'e kadar say�lar olu�turulup rastgele olacak �ekilde kar��t�r�l�r.
@param *arr Say�lar� atayacag�m�z dizi
@param N Dizinin boyutu
*/
void diziOlustur(int *arr,int N){
	int i,randomIndex,temp;
    for(i=0;i<N;i++){    //Birden N'e kadar olan say�lar� diziye s�rayla koyar.
        arr[i]=i+1;
    }
    for(i=0;i<N;i++){	//Rastgele olacak �ekilde dizileri kar��t�r�r.
        randomIndex=rand()%N;
        temp=arr[i];
        arr[i]=arr[randomIndex];
        arr[randomIndex]=temp;
    }
}
/*
@brief Bu fonksiyonda mergeSort i�lemi yap�l�r. Dizi 2'ye b�l�n�r ve kendi aras�nda s�ralama yap�l�r.
@param *arr Dizim
@param left Bir diziyi ikiye par�alarken sol taraf� yani en k���k indis de�erini buraya veririz.
@param right Bir diziyi ikiye par�alarken sa� taraf� yani en b�y�k indis de�erini buraya veririz.
*/
void mergeSort(int *arr,int left,int right){
	int i,j,k,n1,n2,mid,*L,*R;
    if (left<right) {   //Sol taraf sa�dan k���k olmal�
        mid=left+(right-left)/2; //Dizinin ortas�n� buluruz.
        mergeSort(arr,left,mid);  //�kiye par�alad���m�z dizinin ilk par�as�n� bir daha k���ltmeye veririz.
        mergeSort(arr,mid+1,right);  //�kinci par�ay� da k���ltmek i�in tekrar bu fonksiona g�ndeririz.
        n1=mid-left+1;   //�lk par�an�n boyutunu hesaplar�z.
        n2=right-mid;		//�kinci par�an�n boyutunu hesaplar�z.
        L=(int*)malloc(n1*sizeof(int));	//Sol yani ilk par�a dizisini olu�tururuz.
        R=(int*)malloc(n2*sizeof(int)); //Sa� yani ikinci par�a dizisini olu�tururuz.
        for(i=0;i< n1; i++){    //Dizinin sol par�as�n� olu�turdu�umuz diziye atar�z.
            L[i]=arr[left+i];
        }
        for(i=0;i<n2;i++){    //Dizinin sa� par�as�n� olu�turdu�umuz 2.sdiziye atar�z.
            R[i]=arr[mid+1+i];
        }
        i=0;
		j=0;
		k=left;
        while((i<n1)&&(j<n2)){   //�ki dizinin ba��ndaki elemanlar� k�yaslar�z ve hangisi k���kse onu ilk gelen dizimize atar�z.
            if(L[i] <= R[j]){
                arr[k++]=L[i++];
            }
            else{
                arr[k++]=R[j++];
            }
        }
        while(i<n1){        //Herhangi iki dizinin birinde eleman kalmad���nda di�erindeki de�erleri diziye bo�alt�r�z.
            arr[k++]=L[i++];
        }
        while(j<n2){
            arr[k++]=R[j++];
        }
        free(L);				//Olu�turdu�umuz bellek alanlar�n� temizleriz.
        free(R);
    }
}

/*
@brief Bu fonksiyonda ise verilen k say�s�na g�re merge yani birle�tirme yapar�z.
@param **arrays Par�alanm�� alt diziler burada saklan�r.
@param *sizes Bu alt yani par�alanm�� dizilerin boyutlar�n� burada tutar�z.
@param k Ka� par�adan birle�tirece�im bilgisi.
@param *output Alt dizileri s�ralay�p bile�tirmek i�in sonu� dizisi.
@param totalSize Par�alanm�� dizilerimin hepsinin size toplam� di�er bir deyi�le birle�tirilmi� dizimin boyutu.
*/
void kWayMerge(int **arrays,int *sizes,int k,int *output,int totalSize){
	int i,j,minIndex,minValue,*indices;		//indices cursor olarak kullan�r�z.
	indices=(int*)calloc(k, sizeof(int));
    for(i=0;i<totalSize;i++){
        minIndex=-1;
		minValue=INT_MAX;
        for(j=0;j<k;j++){
            if((indices[j]<sizes[j])&&(arrays[j][indices[j]]< minValue)){  //Alt diziler aras�nda cursor olarak g�sterilenlerden k���k olan se�ilir.
                minIndex=j;
                minValue=arrays[j][indices[j]];
            }
        }
        output[i]=minValue;  //S�ralama yapaca��m�z diziye eklenir.
        indices[minIndex]++;
    }
    free(indices);	//�ndisleri tutan dizinin temizlenmesi
}

/*
@brief Bu fonksiyon ise ana fonksiyonumuz say�labilir par�alama i�lemini geri birle�tirme �a�r�lar�n� burda yap�yoruz.
@param *arr S�ras�z dizim
@param *sortedArr arr yani s�ras�z dizinin s�ralanm�� halini tutaca��m dizi.
@param N Toplam ka� eleman oldu�u
@param k Ka� par�aya b�lece�im bilgisi
*/
void kWayMergeSort(int *arr,int *sortedArr,int N,int k){
	int i,j,subArraySize,remaining,**subArrays,*sizes,index=0,currentSize;
    if(k<2){	//Dizi k'dan k���k hale geldiyse daha fazla k'ya b�lemeyiz.Bu durumlarda mergeSort kullanaca��z.
        for(i=0;i<N;i++){    //Ana s�ras�z dizimi kaybetmek istemiyorum.��nk� di�er k'lar ile de i�lemler yapaca��m.
            sortedArr[i]=arr[i];
        }
        mergeSort(sortedArr,0,N-1);
        return;
    }
    subArraySize=N/k;	//Alt dizinin boyutunu buluruz
    remaining=N%k;		//Ne kadar say�n�n artaca��n� buluruz.
    subArrays=(int **)malloc(k*sizeof(int *));
    sizes=(int *)malloc(k*sizeof(int));
    index=0;
    for(i=0;i<k;i++){
        currentSize=subArraySize+(i<remaining?1:0);  //E�er fazlal�k say� say�s� i'den fazla gelirse alt dizi boyutunu +1 geni�letmemiz laz�m.��nk�
        subArrays[i]=(int *)malloc(currentSize*sizeof(int));	// Fazla olan say�lara da alt dizime alabilmek i�in yer a�mam gerekir.
        for (j=0;j<currentSize;j++) {			//Ana dizimdeki elemanlar� alt dizilere atar�z.
            subArrays[i][j]=arr[index++];
        }
        mergeSort(subArrays[i],0,currentSize-1);// alt diziyi (subArrays[i]) s�ralamak i�in mergeSort fonksiyonunu �a��r�r. ��lemin amac�,
        sizes[i]=currentSize;  // kWayMergeSort fonksiyonunda arr dizisini k adet alt diziye b�lmek ve bu alt dizilerin her birini s�ralamakt�r.
    }
    kWayMerge(subArrays,sizes,k,sortedArr,N);  //Ay�rd���m�z dizileri birle�tiririz.
    for(i=0;i<k;i++){
    	free(subArrays[i]);
	}
    free(subArrays); //Par�alanm�� dizilerin ve boyutlar�n�n temizlenmesi
    free(sizes);
}

int main(){
    srand(time(0));
    int i, N;
    clock_t start, end;
    double time_spent;
	printf("N degeri giriniz.\n");
	scanf("%d",&N);
    int *arr=(int*)malloc(N*sizeof(int));
    int *sortedArr=(int*)malloc(N*sizeof(int));
    double time[9]={0}; //9 farkl� zaman de�erini tutmak i�in tan�mlad�k.
    diziOlustur(arr,N);
    /*printf("Orijinal dizi:\n");
    for(i=0;i<N;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");*/
    for(i=2;i<=10;i++){		//Tum k de�erlerini gezece�iz.
    	start=clock(); //Zaman ba�lang�c�
    	kWayMergeSort(arr,sortedArr,N,i);	//kWaySort i�lemi
    	end=clock();	//Zaman biti�i
    	time_spent=((double)(end-start))/CLOCKS_PER_SEC;	//Zaman fark�
    	time[i-2]=time_spent;	//Gerekli indisteki elemana zaman�n at�lmas�.
	}  
    /*printf("Sirali dizi:\n");
    for(i=0;i<N;i++){
        printf("%d ",sortedArr[i]);
    }
    printf("\n");*/
    printf("k degeri icin zamanlar:\n");
    for(i=0;i<9;i++){
    	printf("%.6f\n",time[i]);
    }
    free(arr);		//A�t���m dizilerin temizlenmesi
    free(sortedArr);
    return 0;
}

