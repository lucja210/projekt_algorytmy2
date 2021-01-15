//  Pojedyncza liczba w pliku nie moze miec wiecej niz 8 znakow

//  Dane do sortowania sa pobierane z pliku input_data.txt,
//  po sortowaniu dane sa wpisane do pliku output_data.txt

//  Na potrzeby testu tworzy sie takze plik test_data.txt,
//  ktory przechowuje dane dla obecnie testowanego typu sortowania

//  Rezultaty testow to wpisane do pliku test_results.txt

//
//              ³ucja Telesz
//
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <time.h>
#include <ctime>
#include <chrono>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;
typedef high_resolution_clock Clock;
typedef Clock::time_point ClockTime;

int size = 0;   //rozmiar tablicy
int* table;     //tablica przechowujaca dane
int* test_table;//tablica na potrzeby testow
void gnome_sort() {                                     //sortowanie danych metoda gnoma
    for (int i = 1; i < size; i++) {
        if (i==0) {
            i++;
        }
        if (table[i] < table[i-1]) {
            swap(table[i], table[i-1]);
            i-=2; //jesli algorytm bylby w petli while to zmniejszalibysmy wartosc o jeden
        }
    }
}
void bucket_sort() {                                    //sortowanie danych metoda kubelkowa
    int bucket_value[size], temp_tab[size], temp_index = 0, bucket_amount = 0, status, buckets[100000], bucket[100000]; //inicjalizacja zmiennych
    for (int i = 0; i < size; i++){ //petla przypisujaca dana liczbe do wartosci kubelka
        bucket_value[i] = table[i]/10;
    }
    for (int i = 0; i < size; i++){ //petla sprawdzajaca ilosc potrzebnych kubelkow
        if (i==0){                  //oraz tworzaca tablice ktora zawiera unikatowe wartosci kubelkow
            buckets[bucket_amount]=bucket_value[0];
            bucket_amount++;
            continue;
        }
        status = 0;
        for (int j = 0; j < i; j++){
            if (bucket_value[i]==bucket_value[j]){
                status = 1; //jesli taka wartosc istnieje wczesniej to ten kubelek juz istnieje
            }
        }
        if (status == 0){
            buckets[bucket_amount]=bucket_value[i];
            bucket_amount++;
        }
    }

    sort(buckets, buckets + bucket_amount); //sortowanie kubelkow by ich wartosci byly rosnaco
    for (int i = 0; i < bucket_amount; i++){//przypisanie wartosci do danego kubelka
        int index=0;
        for (int j = 0; j < size; j++){
            if (table[j]/10==buckets[i]){
                bucket[index]=table[j];
                index++;
            }
        }
        if (index>1){
            sort(bucket, bucket+index); //sortowanie wartosci w danym kubelku jesli ilosc wartosci jest wieksza niz jeden
        }
        //cout<<endl<<"Zawartosc kubelka o wartosci: "<<buckets[i]<<endl;
        for (int x = 0; x < index; x++){
            //cout<<bucket[x]<<" ";
            temp_tab[temp_index] = bucket[x];//przypisanie wartosci w odpowiedniej posortowanej kolejnosci do tymczasowej tablicy
            temp_index++;
        }
    }
    //cout<<endl<<endl;
    for (int i = 0; i < size; i++){ //przepisanie tymczasowej tablicy do glownej tablicy
        table[i]=temp_tab[i];
    }
}
void file_write(bool test, string file_name) {          //funkcja odpowiadajaca za odczyt danych z pliku i zapisu jako typ int do glownej tablicy, argument test odpowiada za informacje czy funckcja jest wywolywana dla testow programu
    char write_status;
    if(!test) { //test nie ma potrezby pytac uzytkownika o zapis do pliku
        cout<<"Czy zapisac posortowane liczby do pliku?\nWybieram(t/n): ";
    }
    while(1){
        if(!test) {
        cin>>write_status;
        }
        if (write_status == 't' || test == true) {
            ofstream output_file;
            output_file.open(file_name); //otwarcie pliku
            if(output_file.is_open()) {
                for (int i = 0; i < size; i++){
                    if(test) {
                        output_file<<test_table[i]<<" ";//zapisanie testowych danych
                    } else {
                    output_file<<table[i]<<" ";         //zapisanie docelowych danych
                    }
                }
                if (!test) {
                    system("CLS");
                    cout<<"\n\nDane zostaly zapisane w pliku!\n\n";
                    system("PAUSE");
                }
                output_file.close();//zamkniecie pliku
            } else {
                cout<<"Nie udalo sie otworzyc pliku do zapisu danymi!\n\n";
                system("PAUSE");
            }
            break;
        } else if(write_status == 'n') {
            break;
        } else {
            continue;
        }
    }
}
bool file_read(string file_name){                       //funkcja odczytujaca dane do posortowania z pliku
    table = new int[100000];
    size = 0;
    ifstream input_file;
    input_file.open(file_name); //otwarcie pliku do odczytu
    if (input_file.is_open()){
        char number[10];
        while(input_file>>number) {
            table[size] = atoi(number); //konwersja z typu char na int i wpisanie do tablicy
            size++;                     //ustalenie ilosci danych
        }
        input_file.close();
        return true;
    }
    input_file.close();
    return false;
}
void random_tab(int min, int max) {                     //funkcja losujace tablice do testow
    srand(time(NULL));
    test_table = new int[size];
    for (int i = 0; i < size; i++) {
        test_table[i] = min + rand() % (max+1 - min);
    }
    file_write(true, "test_data.txt");//zapisanie danych do pliku
}
void test(){                                            //testy wydajnosci programu oraz poprawnosci posortowanych danych
    system("CLS");  //czyszczenia okna tarminalu
    int ilosc_testow = 3;   //ilosc testow
    ofstream test_file_output;
    test_file_output.open("test_results.txt");  //otwarcie pliku do odczytu
    if (!test_file_output.is_open()) {  //wyswietlanie i wpis do pliku tekstow
        cout<<"NIE MOZNA ZAPISAC REZULTATOW W PLIKU - BLAD"<<endl<<endl;
    } else {
        test_file_output<<setw(50)<<"LICZBA TESTOW: "<<ilosc_testow<<"\n\n";
    }

    cout<<setw(50)<<"LICZBA TESTOW: "<<ilosc_testow<<"\n\n";
    for (int i = 1; i <= ilosc_testow; i++){
        size = 10000 * i;   //testowy rozmiar tablicy
        cout<<"TEST NR "<<i<<"\nRozmiar tablicy: "<<size<<"\n";
        cout<<setw(35)<<"Sekundy"<<setw(15)<<"Milisekundy"<<setw(15)<<"Nanosekundy"<<endl;
        cout<<setw(20)<<"Gnome sort:";
        if (test_file_output.is_open()) {//wyswietlanie i wpis do pliku tekstow
            test_file_output<<"TEST NR "<<i<<"\nRozmiar tablicy: "<<size<<"\n";
            test_file_output<<setw(35)<<"Sekundy"<<setw(15)<<"Milisekundy"<<setw(15)<<"Nanosekundy"<<endl;
            test_file_output<<setw(20)<<"Gnome sort:";
        }
        random_tab(-100, 100);  //losowanie danych do testu z przedzialu -100 do 100
        file_read("test_data.txt"); //odczyt testowych danych
        ClockTime start_time = Clock::now();    //poczatek mierzenia czasu wykonywanych funkcji
        gnome_sort();               //sortowanie testowych danych
        ClockTime end_time = Clock::now();      //zakonczenie mierzenia czasu dzialania funkcji
        for (int j = 0; j < size-1; j++){   //sprawdzenie poprawnosci sortowania
            if (table[j]>table[j+1]){
                cout<<endl<<endl<<"Program napotkal problem z posortowanymi danymi lecz bedzie kontynuowal dzialanie"<<endl<<endl;
            }
        }

        auto execution_time_sec = duration_cast<seconds>(end_time - start_time).count();    //odczyt czasu wykonywania sortowania w danej jednosce czasu
        auto execution_time_ms = duration_cast<milliseconds>(end_time - start_time).count();
        auto execution_time_ns = duration_cast<nanoseconds>(end_time - start_time).count();
        cout<<setw(15)<<execution_time_sec % 60<<setw(15)<<execution_time_ms % long(1E+3)<<setw(15)<<execution_time_ns % long(1E+6)<<endl;
        cout<<setw(20)<<"Bucket sort:";
        if (test_file_output.is_open()) {//wpis do pliku tekstow
            test_file_output<<setw(15)<<execution_time_sec % 60<<setw(15)<<execution_time_ms % long(1E+3)<<setw(15)<<execution_time_ns % long(1E+6)<<endl;
            test_file_output<<setw(20)<<"Bucket sort:";
        }
        file_read("test_data.txt");
        start_time = Clock::now();
        bucket_sort();
        end_time = Clock::now();
        for (int j = 0; j < size-1; j++){
            if (table[j]>table[j+1]){
                cout<<endl<<endl<<"Program napotkal problem z posortowanymi danymi lecz bedzie kontynuowal dzialanie"<<endl<<endl;
            }
        }
        execution_time_sec = duration_cast<seconds>(end_time - start_time).count();
        execution_time_ms = duration_cast<milliseconds>(end_time - start_time).count();
        execution_time_ns = duration_cast<nanoseconds>(end_time - start_time).count();
        cout<<setw(15)<<execution_time_sec % 60<<setw(15)<<execution_time_ms % long(1E+3)<<setw(15)<<execution_time_ns % long(1E+6)<<endl<<endl;
        if (test_file_output.is_open()) {//wpis do pliku tekstow
            test_file_output<<setw(15)<<execution_time_sec % 60<<setw(15)<<execution_time_ms % long(1E+3)<<setw(15)<<execution_time_ns % long(1E+6)<<endl<<endl;
        }
    }
    test_file_output.close();
    delete[] table; //czyszczenie danych by dane nie zostaly nadpisane
    delete[] test_table;
    size = 0;
    cout<<endl<<endl;
    system("PAUSE");    //zatrzymanie programu do momentu wcisniecia losowago klawisza
}
int main() {                                            //glowna funkcja sterujaca programem
    char action;    //inicjalizacja zmiennej ktora odpowiada za wybor opcji uzytkownika
    while (1){
        system("cls");
        cout<<"Wybierz metode sortowania liczb zawartych w pliku data.txt\n\nWybieram:\na) sortowanie gnoma\nb) sortowanie kubelkowe\n\nt) testy\n\nq) zamknij program\n\nWybieram: ";
        cin>>action;
        if (action == 'q') {
            return 1;   //zakonczenie prorgamu po wybraniu opcji q
        } else if (action == 'a' || action == 'b'){
            system("cls");
            if (file_read("input_data.txt")){   //wywolanie funki file_read
                //wyswietlanie tablicy nieposortowanej
                cout<<"Dane bez sortowania: ";
                for (int i = 0; i < size; i++){ //wyswietlanie danych ktore nie byly sortowane
                    cout<<table[i]<<" ";
                }
                cout<<endl<<endl;
                if(action == 'a') {
                    gnome_sort();   //sortowanie metoda gnoma
                } else {
                    bucket_sort();  //sortowanie metoda kubelkowa
                }

                //wyswietlanie posortowanej tablicy
                cout<<"Posortowane dane: "; //wyswietlenie posortowanych danych
                for (int i = 0; i < size; i++){
                    cout<<table[i]<<" ";
                }
                cout<<endl<<endl;
                file_write(false, "output_data.txt");   //zapis posortowanych danych do pliku

                delete[] table; //czyszczenie pamieci programu
                size = 0;
            } else {
                cout<<"Nie udalo sie otworzyc pliku z danymi!\n\n";
                system("PAUSE");
                return 0;
            }
            continue;
        } else if(action == 't') {
            test(); //wywolanie testow jesli uzytkownik wybierze taka opcje
        } else {
            continue;
        }
    }
    return 0;   //zakonczenie profgramu
}
