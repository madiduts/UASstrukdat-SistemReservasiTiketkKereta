#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> // Untuk generate_id yang lebih acak atau timestamp


#define STATUS_TERSEDIA 0
#define STATUS_TERISI 1

typedef struct kereta {
    int id, harga; 
    char kodeKereta[50], rute[50], jadwalKeberangkatan[50];
} kereta;

typedef struct kursi {
    int nomorKursi, status; // 0 = Tersedia, 1 = Terisi
} kursi;

typedef struct penumpang {
    int idPenumpang; // Bisa di-generate otomatis
    char namaLengkap[100], nik[20], noTelepon[20], email[50];
} penumpang;

typedef struct tiket{
    int totalBayar;
    char kodeBooking[15], namaPenumpang[20], tanggalKeberangkatan[20], waktuKeberangkatan[10];
    penumpang dataPenumpang; // Data penumpang untuk tiket ini
} tiket;

typedef struct stasiun {
    int idStasiun;
    char kodeKereta[50], kotaAsal[50], kotaTujuan[50];
} stasiun;

typedef struct jadwalKereta {
    int hargaFirstClass, hargaBisnis, hargaEkonomi;
    char kodeKereta[50], waktuBerangkat[10], waktuTiba[10];
    stasiun kota; 
} jadwalKereta;

typedef struct node {
    tiket dataTiket;
    struct node *next;
} node;

void inisialisasiSistem();
void customerMenu();
void registerCustomer();
void loginCustomer();
void tampilkanJadwal();
void cariJadwal();
void bookingTiket();
void lihatPesanan();
void batalkanBooking();
void lihatHistoryBooking();
void tampilkanPetaKursi(Schedule* schedule);
Customer* cariPenumpang(int id);
Schedule* cariJadwal(int id);
Seat* buatlistKursi(int totalSeats);
void pushHistory(int bookingId, int customerId, int scheduleId, int seatNumber, char* customerName, char* route, float price, char* status);
void batalkanPemesanan(int bookingId, char* customerName, char* reason);
BookingNode* tambahkanNodeBooking(BookingNode* root, int bookingId, int customerId, Schedule* schedule, Customer* customer, int seatNumber, char* status);
BookingNode* cariNodeBooking(BookingNode* root, int bookingId);
BookingNode* cariPesananPenumpang(BookingNode* root, int customerId);
void tampilkanPesananPenumpang(BookingNode* root, int customerId);
void tanggalSekarang(char* buffer);
void waktuSekaranf(char* buffer);
