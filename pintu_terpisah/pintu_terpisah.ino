// masukkan library yang digunakan
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// definisi pin untuk sensor
#define sensor_masuk    13
#define sensor_keluar   12

// definisi pin untuk dfplayer
#define busy_pin        14

// definisi status saat audio diputar
bool sedang_memutar_audio = false;

// definisi status pengunjung masuk dan keluar
bool pengunjung_masuk  = false;
bool pengunjung_keluar = false;

// atur pin modul audio
SoftwareSerial mySoftwareSerial(4,5);
DFRobotDFPlayerMini audio;


// bagian konfigurasi
void setup(void) 
{
    // atur komunikasi serial dengan modul audio
    mySoftwareSerial.begin(9600);

    // atur komunikasi serial dengan komputer (opsional)
    Serial.begin(9600);
    
    // atur sensor sebagai input
    pinMode(sensor_masuk,  INPUT_PULLUP);
    pinMode(sensor_keluar, INPUT_PULLUP);
    
    pinMode(busy_pin, INPUT);

    // aktifkan modul audio
    audio.begin(mySoftwareSerial);
    delay(2000);

    // atur waktu timeout selama 1000 ms
    audio.setTimeOut(1000); 

    // atur volume suara
    audio.volume(20);
    audio.EQ(DFPLAYER_EQ_NORMAL);

    // atur sumber file audio
    audio.outputDevice(DFPLAYER_DEVICE_SD);
}


// bagian loop - program utama
void loop(void) 
{
    // cek apakah sedang memutar audio atau tidak
    if(digitalRead(busy_pin) == LOW)
    {
        sedang_memutar_audio = true;
    }
    else
    {
        // tunggu 3 detik untuk audio selanjutnya
        if(sedang_memutar_audio == true)
        {
            delay(3000);
        }
        
        sedang_memutar_audio = false;
    }
    
    // jika sensor pintu masuk aktif
    if(digitalRead(sensor_masuk) == LOW && sedang_memutar_audio == false)
    {
        // atur pengunjung sedang masuk
        pengunjung_masuk = true;
        
        // tunggu sampai pengunjung masuk
        while(pengunjung_masuk == true)
        {
            pengunjung_masuk = (digitalRead(sensor_masuk)) ? false : true;
            delay(100);
        }
        
        // putar audio "selamat datang"
        audio.play(1);
    }

    // jika sensor keluar aktif
    if(digitalRead(sensor_keluar) == LOW && sedang_memutar_audio == false)
    {
        // putar audio "terimakasih"
        audio.play(2);
        
        // atur pengunjung sedang masuk
        pengunjung_keluar = true;
        
        // tunggu sampai pengunjung masuk
        while(pengunjung_keluar == true)
        {
            pengunjung_keluar = (digitalRead(sensor_keluar)) ? false : true;
            delay(100);
        }
    }

    // berikan delay supaya stabil 
    delay(50);
}
