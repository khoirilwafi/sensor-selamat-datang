// masukkan library yang digunakan
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// definisi pin untuk sensor
#define sensor_luar    11
#define sensor_luar    12

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
    pinMode(sensor_luar,  INPUT_PULLUP);
    pinMode(sensor_dalam, INPUT_PULLUP);

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
    // jika sensor luar aktif
    if(digitalRead(sensor_luar) == LOW)
    {
        // kemudian sensor dalam aktif
        if(digitalRead(sensor_dalam) == LOW)
        {
            // berarti pengunjung masuk
            // putar audio "selamat datang"
            myDFPlayer.play(1);
            
            // atur pengunjung sedang masuk
            pengunjung_masuk = true;

            // tunggu sampai pengunjung masuk
            while(pengunjung_masuk == true)
            {
                pengunjung_masuk = (digitalRead(sensor_dalam)) ? false : true;
                delay(100);
            }

            // berikan jeda waktu 2 detik
            delay(2000);
        }
    }

    // jika sensor dalam aktif
    else if(digitalRead(sensor_dalam) == LOW)
    {
        // kemudian sensor luar aktif
        if(digitalRead(sensor_luar) == LOW)
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
