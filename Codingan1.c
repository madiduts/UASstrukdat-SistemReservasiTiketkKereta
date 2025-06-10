#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// Struktur data untuk menyimpan informasi kereta, gerbong, kursi, penumpang, dan tiket

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
    int total_harga;

    char tanggal_pemesanan[20];  
    char tanggal_keberangkatan[20]; 
    char waktu_keberangkatan[10];   
    char status[20];              // "Pending", "Confirmed", "Cancelled", dll.

    penumpang data_penumpang; // Setiap tiket harus terkait dengan penumpangnya
} tiket;

// Fungsi menambahkan data

void tambah_kereta(){


}

void tambah_gerbong(){


}

void buat_reservasi(){


}

// Fungsi untuk menghapus data 

void hapus_kereta(){
    
}

void batalkan_reservasi(){

}

// Fungsi untuk mencari data

void cari_kereta_by_rute(){

}

void cari_tiket_by_id(){

}

void cari_tiket_by_penumpang(){

}

void cek_ketersediaan_kursi(){

}

// Fungsi untuk menampilkan data

void tampilkan_daftar_kereta(){
    
}

void cetak_tiket(){


}

void tampilkan_ketersediaan_kursi(){

}

void tampilkan_riwayat_tiket(){

}

// Fungsi inisialisasi data

void muatData_dariFile(){

}

void simpanData_keFile(){

}

// Fungsi tambahan

void validasi_input(){


}

void tampilkan_menu(){

}

int generate_id(){

}





























int main(){


}
