// masukkan library yang digunakan
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// definisi pin untuk sensor
#define sensor_masuk    11
#define sensor_keluar   12

// definisi status pengunjung masuk dan keluar
#define pengunjung_masuk  = false;
#define pengunjung_keluar = false;

// atur pin modul audio
SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;


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

    // aktifkan modul audio
    if(!myDFPlayer.begin(mySoftwareSerial)) 
    {
        Serial.println("modul audio gagal");
        while(true);
    }
    else
    {
        Serial.println("modul audio berhasil");
    }

    // atur waktu timeout selama 500 ms
    myDFPlayer.setTimeOut(500); 

    // atur volume suara
    myDFPlayer.volume(10);
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

    // atur sumber file audio
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}


// bagian loop - program utama
void loop(void) 
{
    // baca sensor
    // jika sensor pintu masuk aktif
    if(digitalRead(sensor_masuk) == LOW)
    {
        // berarti pengunjung masuk
        // putar audio "selamat datang"
        myDFPlayer.play(1);
        
        // atur pengunjung sedang masuk
        pengunjung_masuk = true;

        // tunggu sampai pengunjung masuk
        while(pengunjung_masuk == true)
        {
            pengunjung_masuk = (digitalRead(sensor_masuk)) ? false : true;
            delay(100);
        }

        // berikan jeda waktu 2 detik
        delay(2000);
    }

    // jika sensor dalam aktif
    else if(digitalRead(sensor_keluar) == LOW)
    {
        // berarti pengunjung keluar
        // putar audio "sampai jumpa lagi"
        myDFPlayer.play(2);
        
        // atur pengunjung sedang keluar
        pengunjung_keluar = true;

        // tunggu sampai pengunjung keluar
        while(pengunjung_keluar == true)
        {
            pengunjung_keluar = (digitalRead(sensor_luar)) ? false : true;
            delay(100);
        }

        // berikan jeda waktu 2 detik
        delay(2000);
    }

    // jika kedua sensor tidak aktif
    else
    {
        // atur tidak ada pengunjung masuk/keluar
        pengunjung_masuk  = false;
        pengunjung_keluar = false;
    }
    
    // kasih delay biar aman
    delay(10);
}
