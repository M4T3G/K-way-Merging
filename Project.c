#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
/*
@brief Bu fonksiyonda 1'den N'e kadar sayýlar oluþturulup rastgele olacak þekilde karýþtýrýlýr.
@param *arr Sayýlarý atayacagýmýz dizi
@param N Dizinin boyutu
*/
void diziOlustur(int *arr,int N){
	int i,randomIndex,temp;
    for(i=0;i<N;i++){    //Birden N'e kadar olan sayýlarý diziye sýrayla koyar.
        arr[i]=i+1;
    }
    for(i=0;i<N;i++){	//Rastgele olacak þekilde dizileri karýþtýrýr.
        randomIndex=rand()%N;
        temp=arr[i];
        arr[i]=arr[randomIndex];
        arr[randomIndex]=temp;
    }
}
/*
@brief Bu fonksiyonda mergeSort iþlemi yapýlýr. Dizi 2'ye bölünür ve kendi arasýnda sýralama yapýlýr.
@param *arr Dizim
@param left Bir diziyi ikiye parçalarken sol tarafý yani en küçük indis deðerini buraya veririz.
@param right Bir diziyi ikiye parçalarken sað tarafý yani en büyük indis deðerini buraya veririz.
*/
void mergeSort(int *arr,int left,int right){
	int i,j,k,n1,n2,mid,*L,*R;
    if (left<right) {   //Sol taraf saðdan küçük olmalý
        mid=left+(right-left)/2; //Dizinin ortasýný buluruz.
        mergeSort(arr,left,mid);  //Ýkiye parçaladýðýmýz dizinin ilk parçasýný bir daha küçültmeye veririz.
        mergeSort(arr,mid+1,right);  //Ýkinci parçayý da küçültmek için tekrar bu fonksiona göndeririz.
        n1=mid-left+1;   //Ýlk parçanýn boyutunu hesaplarýz.
        n2=right-mid;		//Ýkinci parçanýn boyutunu hesaplarýz.
        L=(int*)malloc(n1*sizeof(int));	//Sol yani ilk parça dizisini oluþtururuz.
        R=(int*)malloc(n2*sizeof(int)); //Sað yani ikinci parça dizisini oluþtururuz.
        for(i=0;i< n1; i++){    //Dizinin sol parçasýný oluþturduðumuz diziye atarýz.
            L[i]=arr[left+i];
        }
        for(i=0;i<n2;i++){    //Dizinin sað parçasýný oluþturduðumuz 2.sdiziye atarýz.
            R[i]=arr[mid+1+i];
        }
        i=0;
		j=0;
		k=left;
        while((i<n1)&&(j<n2)){   //Ýki dizinin baþýndaki elemanlarý kýyaslarýz ve hangisi küçükse onu ilk gelen dizimize atarýz.
            if(L[i] <= R[j]){
                arr[k++]=L[i++];
            }
            else{
                arr[k++]=R[j++];
            }
        }
        while(i<n1){        //Herhangi iki dizinin birinde eleman kalmadýðýnda diðerindeki deðerleri diziye boþaltýrýz.
            arr[k++]=L[i++];
        }
        while(j<n2){
            arr[k++]=R[j++];
        }
        free(L);				//Oluþturduðumuz bellek alanlarýný temizleriz.
        free(R);
    }
}

/*
@brief Bu fonksiyonda ise verilen k sayýsýna göre merge yani birleþtirme yaparýz.
@param **arrays Parçalanmýþ alt diziler burada saklanýr.
@param *sizes Bu alt yani parçalanmýþ dizilerin boyutlarýný burada tutarýz.
@param k Kaç parçadan birleþtireceðim bilgisi.
@param *output Alt dizileri sýralayýp bileþtirmek için sonuç dizisi.
@param totalSize Parçalanmýþ dizilerimin hepsinin size toplamý diðer bir deyiþle birleþtirilmiþ dizimin boyutu.
*/
void kWayMerge(int **arrays,int *sizes,int k,int *output,int totalSize){
	int i,j,minIndex,minValue,*indices;		//indices cursor olarak kullanýrýz.
	indices=(int*)calloc(k, sizeof(int));
    for(i=0;i<totalSize;i++){
        minIndex=-1;
		minValue=INT_MAX;
        for(j=0;j<k;j++){
            if((indices[j]<sizes[j])&&(arrays[j][indices[j]]< minValue)){  //Alt diziler arasýnda cursor olarak gösterilenlerden küçük olan seçilir.
                minIndex=j;
                minValue=arrays[j][indices[j]];
            }
        }
        output[i]=minValue;  //Sýralama yapacaðýmýz diziye eklenir.
        indices[minIndex]++;
    }
    free(indices);	//Ýndisleri tutan dizinin temizlenmesi
}

/*
@brief Bu fonksiyon ise ana fonksiyonumuz sayýlabilir parçalama iþlemini geri birleþtirme çaðrýlarýný burda yapýyoruz.
@param *arr Sýrasýz dizim
@param *sortedArr arr yani sýrasýz dizinin sýralanmýþ halini tutacaðým dizi.
@param N Toplam kaç eleman olduðu
@param k Kaç parçaya böleceðim bilgisi
*/
void kWayMergeSort(int *arr,int *sortedArr,int N,int k){
	int i,j,subArraySize,remaining,**subArrays,*sizes,index=0,currentSize;
    if(k<2){	//Dizi k'dan küçük hale geldiyse daha fazla k'ya bölemeyiz.Bu durumlarda mergeSort kullanacaðýz.
        for(i=0;i<N;i++){    //Ana sýrasýz dizimi kaybetmek istemiyorum.Çünkü diðer k'lar ile de iþlemler yapacaðým.
            sortedArr[i]=arr[i];
        }
        mergeSort(sortedArr,0,N-1);
        return;
    }
    subArraySize=N/k;	//Alt dizinin boyutunu buluruz
    remaining=N%k;		//Ne kadar sayýnýn artacaðýný buluruz.
    subArrays=(int **)malloc(k*sizeof(int *));
    sizes=(int *)malloc(k*sizeof(int));
    index=0;
    for(i=0;i<k;i++){
        currentSize=subArraySize+(i<remaining?1:0);  //Eðer fazlalýk sayý sayýsý i'den fazla gelirse alt dizi boyutunu +1 geniþletmemiz lazým.Çünkü
        subArrays[i]=(int *)malloc(currentSize*sizeof(int));	// Fazla olan sayýlara da alt dizime alabilmek için yer açmam gerekir.
        for (j=0;j<currentSize;j++) {			//Ana dizimdeki elemanlarý alt dizilere atarýz.
            subArrays[i][j]=arr[index++];
        }
        mergeSort(subArrays[i],0,currentSize-1);// alt diziyi (subArrays[i]) sýralamak için mergeSort fonksiyonunu çaðýrýr. Ýþlemin amacý,
        sizes[i]=currentSize;  // kWayMergeSort fonksiyonunda arr dizisini k adet alt diziye bölmek ve bu alt dizilerin her birini sýralamaktýr.
    }
    kWayMerge(subArrays,sizes,k,sortedArr,N);  //Ayýrdýðýmýz dizileri birleþtiririz.
    for(i=0;i<k;i++){
    	free(subArrays[i]);
	}
    free(subArrays); //Parçalanmýþ dizilerin ve boyutlarýnýn temizlenmesi
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
    double time[9]={0}; //9 farklý zaman deðerini tutmak için tanýmladýk.
    diziOlustur(arr,N);
    /*printf("Orijinal dizi:\n");
    for(i=0;i<N;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");*/
    for(i=2;i<=10;i++){		//Tum k deðerlerini gezeceðiz.
    	start=clock(); //Zaman baþlangýcý
    	kWayMergeSort(arr,sortedArr,N,i);	//kWaySort iþlemi
    	end=clock();	//Zaman bitiþi
    	time_spent=((double)(end-start))/CLOCKS_PER_SEC;	//Zaman farký
    	time[i-2]=time_spent;	//Gerekli indisteki elemana zamanýn atýlmasý.
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
    free(arr);		//Açtýðým dizilerin temizlenmesi
    free(sortedArr);
    return 0;
}

