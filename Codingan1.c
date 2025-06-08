#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct kereta {
        int id, harga, total_kapasitas;
        char nama[50], rute [50], jadwal [50];
    } kereta;

typedef struct kursi {
    int nomor_kursi, status;
} kursi;

typedef struct gerbong {
    int id_gerbong, kapasitas_total, kursi_terisi;
    char nama_gerbong[50];
    kursi *daftar_kursi;
} gerbong;

typedef struct penumpang {
    int id_penumpang, umur;
    char nama[50], no_telepon[20];
} penumpang;

typedef struct tiket {
    int id_tiket;
    int id_kereta_terpilih; 
    int id_gerbong_terpilih;
    int nomor_kursi_terpilih;
    penumpang data_penumpang; // Setiap tiket harus terkait dengan penumpangnya
    int total_harga;
    char tanggal_pemesanan[20];  
    char tanggal_keberangkatan[20]; 
    char waktu_keberangkatan[10];   
    char status[20];              // "Pending", "Confirmed", "Cancelled", dll.
} tiket;


