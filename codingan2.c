#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Untuk tolower

#define MAX_NAMA 100
#define MAX_KURSI 100

// Struktur untuk data pelanggan
typedef struct Pelanggan {
    int id;
    char nama[MAX_NAMA];
    char telepon[15];
    char email[MAX_NAMA];
    char alamat[MAX_NAMA];
    struct Pelanggan* next;
} Pelanggan;

// Struktur untuk kursi kereta
typedef struct Kursi {
    int nomorKursi;
    int isTerisi;
    Pelanggan* pelanggan;
    struct Kursi* next;
} Kursi;

// Struktur untuk jadwal kereta (data statis)
typedef struct JadwalKereta {
    int idJadwal;
    char keberangkatan[MAX_NAMA];
    char tujuan[MAX_NAMA];
    char waktuBerangkat[10];
    char waktuTiba[10];
    char tanggal[15];
    float harga;
    int totalKursi;
    int kursiTersedia;
    Kursi* daftarKursi;
    struct JadwalKereta* next;
} JadwalKereta;

// Struktur untuk riwayat pemesanan pelanggan (Stack)
typedef struct RiwayatPemesanan {
    int idPemesanan;
    int idPelanggan;
    int idJadwal;
    int nomorKursi;
    char namaPelanggan[MAX_NAMA];
    char rute[MAX_NAMA];
    char tanggalPemesanan[20];
    char status[20];
    float totalHarga;
    struct RiwayatPemesanan* next;
} RiwayatPemesanan;

// Struktur untuk antrian pembatalan (Queue)
typedef struct AntrianPembatalan {
    int idPemesanan;
    char namaPelanggan[MAX_NAMA];
    char alasan[MAX_NAMA];
    char tanggalPermintaan[20];
    struct AntrianPembatalan* next;
} AntrianPembatalan;

// Tree untuk pencarian cepat pemesanan
typedef struct NodePemesanan {
    int idPemesanan;
    int idPelanggan;
    JadwalKereta* jadwal;
    Pelanggan* pelanggan;
    int nomorKursi;
    char tanggalPemesanan[20];
    char status[20];
    struct NodePemesanan* left;
    struct NodePemesanan* right;
} NodePemesanan;

// Global variables
JadwalKereta* headJadwal = NULL;
Pelanggan* headPelanggan = NULL;
RiwayatPemesanan* stackRiwayat = NULL;
AntrianPembatalan* headPembatalan = NULL;
AntrianPembatalan* tailPembatalan = NULL;
NodePemesanan* rootPohonPemesanan = NULL;
int nextIdPemesanan = 1001;
int nextIdPelanggan = 1;

// Function prototypes
void inisialisasiSistem();
void menuPelanggan();
void daftarPelanggan();
void loginPelanggan();
void tampilkanJadwal();
void searchJadwal();
void pesanTiket();
void lihatPemesananSaya();
void batalkanPemesananSaya();
void lihatRiwayatPemesanan();
void tampilkanPetaKursi(JadwalKereta* jadwal);
Pelanggan* cariPelangganById(int id);
JadwalKereta* searchJadwalById(int id);
Kursi* buatDaftarKursi(int totalKursi);
void pushRiwayat(int idPemesanan, int idPelanggan, int idJadwal, int nomorKursi, char* namaPelanggan, char* rute, float harga, char* status);
void enqueuePembatalan(int idPemesanan, char* namaPelanggan, char* alasan);
NodePemesanan* insertNodePemesanan(NodePemesanan* root, int idPemesanan, int idPelanggan, JadwalKereta* jadwal, Pelanggan* pelanggan, int nomorKursi, char* status);
NodePemesanan* searchNodePemesanan(NodePemesanan* root, int idPemesanan);
void tampilkanPemesananPelanggan(NodePemesanan* root, int idPelanggan);
void getTanggalSaatIni(char* buffer);
void getWaktuSaatIni(char* buffer);

// Global variable untuk pelanggan yang sedang login
Pelanggan* pelangganSaatIni = NULL;

int main() {
    inisialisasiSistem();
    menuPelanggan();
    return 0;
}

void inisialisasiSistem() {
    printf("=== SISTEM PEMESANAN TIKET KERETA WHOOSH ===\n");
    printf("Selamat datang di layanan pemesanan tiket Whoosh!\n");
    printf("Menginisialisasi sistem...\n\n");
    
    // Inisialisasi jadwal kereta (data statis)
    JadwalKereta* jadwal1 = (JadwalKereta*)malloc(sizeof(JadwalKereta));
    jadwal1->idJadwal = 1;
    strcpy(jadwal1->keberangkatan, "Halim");
    strcpy(jadwal1->tujuan, "Padalarang");
    strcpy(jadwal1->waktuBerangkat, "06:00");
    strcpy(jadwal1->waktuTiba, "07:30");
    strcpy(jadwal1->tanggal, "2025-06-15");
    jadwal1->harga = 150000;
    jadwal1->totalKursi = 50;
    jadwal1->kursiTersedia = 50;
    jadwal1->daftarKursi = buatDaftarKursi(50);
    jadwal1->next = NULL;
    headJadwal = jadwal1;
    
    JadwalKereta* jadwal2 = (JadwalKereta*)malloc(sizeof(JadwalKereta));
    jadwal2->idJadwal = 2;
    strcpy(jadwal2->keberangkatan, "Padalarang");
    strcpy(jadwal2->tujuan, "Halim");
    strcpy(jadwal2->waktuBerangkat, "08:00");
    strcpy(jadwal2->waktuTiba, "09:30");
    strcpy(jadwal2->tanggal, "2025-06-15");
    jadwal2->harga = 150000;
    jadwal2->totalKursi = 50;
    jadwal2->kursiTersedia = 50;
    jadwal2->daftarKursi = buatDaftarKursi(50);
    jadwal2->next = headJadwal;
    headJadwal = jadwal2;
    
    JadwalKereta* jadwal3 = (JadwalKereta*)malloc(sizeof(JadwalKereta));
    jadwal3->idJadwal = 3;
    strcpy(jadwal3->keberangkatan, "Halim");
    strcpy(jadwal3->tujuan, "Padalarang");
    strcpy(jadwal3->waktuBerangkat, "10:00");
    strcpy(jadwal3->waktuTiba, "11:30");
    strcpy(jadwal3->tanggal, "2025-06-15");
    jadwal3->harga = 150000;
    jadwal3->totalKursi = 50;
    jadwal3->kursiTersedia = 50;
    jadwal3->daftarKursi = buatDaftarKursi(50);
    jadwal3->next = headJadwal;
    headJadwal = jadwal3;
    
    JadwalKereta* jadwal4 = (JadwalKereta*)malloc(sizeof(JadwalKereta));
    jadwal4->idJadwal = 4;
    strcpy(jadwal4->keberangkatan, "Padalarang");
    strcpy(jadwal4->tujuan, "Halim");
    strcpy(jadwal4->waktuBerangkat, "15:00");
    strcpy(jadwal4->waktuTiba, "16:30");
    strcpy(jadwal4->tanggal, "2025-06-15");
    jadwal4->harga = 150000;
    jadwal4->totalKursi = 50;
    jadwal4->kursiTersedia = 50;
    jadwal4->daftarKursi = buatDaftarKursi(50);
    jadwal4->next = headJadwal;
    headJadwal = jadwal4;
    
    printf("Sistem berhasil diinisialisasi!\n");
    printf("Tersedia %d jadwal kereta untuk hari ini.\n\n", 4);
}

void menuPelanggan() {
    int pilihan;
    
    while(1) {
        if(pelangganSaatIni == NULL) {
            printf("=== SELAMAT DATANG DI WHOOSH ===\n");
            printf("1. Daftar Akun Baru\n");
            printf("2. Login\n");
            printf("3. Lihat Jadwal Kereta\n");
            printf("4. Cari Jadwal\n");
            printf("0. Keluar\n");
            printf("Pilihan: ");
            scanf("%d", &pilihan);
            getchar(); // Membersihkan newline character
            
            switch(pilihan) {
                case 1:
                    daftarPelanggan();
                    break;
                case 2:
                    loginPelanggan();
                    break;
                case 3:
                    tampilkanJadwal();
                    break;
                case 4:
                    searchJadwal();
                    break;
                case 0:
                    printf("Terima kasih telah menggunakan layanan Whoosh!\n");
                    exit(0);
                default:
                    printf("Pilihan tidak valid!\n");
            }
        } else {
            printf("\n=== MENU PELANGGAN - %s ===\n", pelangganSaatIni->nama);
            printf("1. Lihat Jadwal Kereta\n");
            printf("2. Cari Jadwal\n");
            printf("3. Pesan Tiket\n");
            printf("4. Lihat Pemesanan Saya\n");
            printf("5. Batalkan Pemesanan\n");
            printf("6. Riwayat Pemesanan\n");
            printf("7. Logout\n");
            printf("0. Keluar\n");
            printf("Pilihan: ");
            scanf("%d", &pilihan);
            getchar(); // Membersihkan newline character
            
            switch(pilihan) {
                case 1:
                    tampilkanJadwal();
                    break;
                case 2:
                    searchJadwal();
                    break;
                case 3:
                    pesanTiket();
                    break;
                case 4:
                    lihatPemesananSaya();
                    break;
                case 5:
                    batalkanPemesananSaya();
                    break;
                case 6:
                    lihatRiwayatPemesanan();
                    break;
                case 7:
                    pelangganSaatIni = NULL;
                    printf("Anda telah logout.\n");
                    break;
                case 0:
                    printf("Terima kasih telah menggunakan layanan Whoosh!\n");
                    exit(0);
                default:
                    printf("Pilihan tidak valid!\n");
            }
        }
    }
}

void daftarPelanggan() {
    Pelanggan* newPelanggan = (Pelanggan*)malloc(sizeof(Pelanggan));
    
    printf("\n=== DAFTAR AKUN BARU ===\n");
    newPelanggan->id = nextIdPelanggan++;
    
    printf("Nama Lengkap: ");
    fgets(newPelanggan->nama, MAX_NAMA, stdin);
    newPelanggan->nama[strcspn(newPelanggan->nama, "\n")] = 0;
    
    printf("Nomor Telepon: ");
    fgets(newPelanggan->telepon, 15, stdin);
    newPelanggan->telepon[strcspn(newPelanggan->telepon, "\n")] = 0;
    
    printf("Email: ");
    fgets(newPelanggan->email, MAX_NAMA, stdin);
    newPelanggan->email[strcspn(newPelanggan->email, "\n")] = 0;
    
    printf("Alamat: ");
    fgets(newPelanggan->alamat, MAX_NAMA, stdin);
    newPelanggan->alamat[strcspn(newPelanggan->alamat, "\n")] = 0;
    
    newPelanggan->next = headPelanggan;
    headPelanggan = newPelanggan;
    
    printf("\n=== PENDAFTARAN BERHASIL ===\n");
    printf("ID Pelanggan: %d\n", newPelanggan->id);
    printf("Nama: %s\n", newPelanggan->nama);
    printf("Silakan login untuk melanjutkan.\n");
}

void loginPelanggan() {
    int idPelanggan;
    char telepon[15];
    
    printf("\n=== LOGIN PELANGGAN ===\n");
    printf("ID Pelanggan: ");
    scanf("%d", &idPelanggan);
    getchar(); // Membersihkan newline character
    
    printf("Nomor Telepon: ");
    fgets(telepon, 15, stdin);
    telepon[strcspn(telepon, "\n")] = 0;
    
    Pelanggan* pelanggan = cariPelangganById(idPelanggan);
    
    if(pelanggan != NULL && strcmp(pelanggan->telepon, telepon) == 0) {
        pelangganSaatIni = pelanggan;
        printf("\n=== LOGIN BERHASIL ===\n");
        printf("Selamat datang, %s!\n", pelanggan->nama);
    } else {
        printf("ID Pelanggan atau nomor telepon salah!\n");
    }
}

void tampilkanJadwal() {
    printf("\n=== JADWAL KERETA WHOOSH HARI INI ===\n");
    printf("%-4s %-15s %-15s %-8s %-8s %-12s %-10s %-8s\n", 
           "ID", "Keberangkatan", "Tujuan", "Berangkat", "Tiba", "Tanggal", "Harga", "Tersedia");
    printf("-----------------------------------------------------------------------------------------\n");
    
    JadwalKereta* current = headJadwal;
    while(current != NULL) {
        printf("%-4d %-15s %-15s %-8s %-8s %-12s Rp%-8.0f %-8d\n",
               current->idJadwal, current->keberangkatan, current->tujuan,
               current->waktuBerangkat, current->waktuTiba, current->tanggal,
               current->harga, current->kursiTersedia);
        current = current->next;
    }
    
    printf("\nKeterangan: Semua jadwal untuk tanggal %s\n", "2025-06-15");
}

void searchJadwal() {
    char keberangkatan[MAX_NAMA], tujuan[MAX_NAMA];
    
    printf("\n=== CARI JADWAL KERETA ===\n");
    printf("Stasiun Keberangkatan: ");
    fgets(keberangkatan, MAX_NAMA, stdin);
    keberangkatan[strcspn(keberangkatan, "\n")] = 0;
    
    printf("Stasiun Tujuan: ");
    fgets(tujuan, MAX_NAMA, stdin);
    tujuan[strcspn(tujuan, "\n")] = 0;
    
    printf("\n=== HASIL PENCARIAN ===\n");
    printf("Rute: %s -> %s\n", keberangkatan, tujuan);
    printf("%-4s %-8s %-8s %-12s %-10s %-8s\n", 
           "ID", "Berangkat", "Tiba", "Tanggal", "Harga", "Tersedia");
    printf("---------------------------------------------------------------\n");
    
    JadwalKereta* current = headJadwal;
    int ditemukan = 0;
    
    while(current != NULL) {
        // Menggunakan tolower untuk perbandingan case-insensitive
        char temp_keberangkatan_current[MAX_NAMA];
        char temp_keberangkatan_input[MAX_NAMA];
        char temp_tujuan_current[MAX_NAMA];
        char temp_tujuan_input[MAX_NAMA];

        for(int i = 0; current->keberangkatan[i]; i++) temp_keberangkatan_current[i] = tolower(current->keberangkatan[i]);
        temp_keberangkatan_current[strlen(current->keberangkatan)] = '\0';
        for(int i = 0; keberangkatan[i]; i++) temp_keberangkatan_input[i] = tolower(keberangkatan[i]);
        temp_keberangkatan_input[strlen(keberangkatan)] = '\0';

        for(int i = 0; current->tujuan[i]; i++) temp_tujuan_current[i] = tolower(current->tujuan[i]);
        temp_tujuan_current[strlen(current->tujuan)] = '\0';
        for(int i = 0; tujuan[i]; i++) temp_tujuan_input[i] = tolower(tujuan[i]);
        temp_tujuan_input[strlen(tujuan)] = '\0';


        if(strcmp(temp_keberangkatan_current, temp_keberangkatan_input) == 0 && 
           strcmp(temp_tujuan_current, temp_tujuan_input) == 0) {
            printf("%-4d %-8s %-8s %-12s Rp%-8.0f %-8d\n",
                   current->idJadwal, current->waktuBerangkat, current->waktuTiba,
                   current->tanggal, current->harga, current->kursiTersedia);
            ditemukan = 1;
        }
        current = current->next;
    }
    
    if(!ditemukan) {
        printf("Tidak ada jadwal untuk rute tersebut.\n");
    }
}

void pesanTiket() {
    if(pelangganSaatIni == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    int idJadwal, nomorKursi;
    char konfirmasi;
    
    printf("\n=== PESAN TIKET KERETA ===\n");
    tampilkanJadwal();
    
    printf("\nMasukkan ID Jadwal yang ingin dipesan: ");
    scanf("%d", &idJadwal);
    
    JadwalKereta* jadwal = searchJadwalById(idJadwal);
    if(jadwal == NULL) {
        printf("Jadwal tidak ditemukan!\n");
        return;
    }
    
    if(jadwal->kursiTersedia <= 0) {
        printf("Maaf, tiket sudah habis untuk jadwal ini!\n");
        return;
    }
    
    printf("\n=== DETAIL JADWAL ===\n");
    printf("Rute: %s -> %s\n", jadwal->keberangkatan, jadwal->tujuan);
    printf("Tanggal: %s\n", jadwal->tanggal);
    printf("Waktu: %s - %s\n", jadwal->waktuBerangkat, jadwal->waktuTiba);
    printf("Harga: Rp %.0f\n", jadwal->harga);
    printf("Kursi Tersedia: %d\n", jadwal->kursiTersedia);
    
    // Tampilkan peta kursi
    tampilkanPetaKursi(jadwal);
    
    printf("\nPilih nomor kursi (1-%d): ", jadwal->totalKursi);
    scanf("%d", &nomorKursi);
    
    if(nomorKursi < 1 || nomorKursi > jadwal->totalKursi) {
        printf("Nomor kursi tidak valid!\n");
        return;
    }
    
    // Cek ketersediaan kursi
    Kursi* kursi = jadwal->daftarKursi;
    while(kursi != NULL && kursi->nomorKursi != nomorKursi) {
        kursi = kursi->next;
    }
    
    if(kursi == NULL || kursi->isTerisi) {
        printf("Kursi nomor %d sudah terisi!\n", nomorKursi);
        return;
    }
    
    // Konfirmasi pemesanan
    printf("\n=== KONFIRMASI PEMESANAN ===\n");
    printf("Nama: %s\n", pelangganSaatIni->nama);
    printf("Telepon: %s\n", pelangganSaatIni->telepon);
    printf("Rute: %s -> %s\n", jadwal->keberangkatan, jadwal->tujuan);
    printf("Tanggal: %s\n", jadwal->tanggal);
    printf("Waktu: %s - %s\n", jadwal->waktuBerangkat, jadwal->waktuTiba);
    printf("Kursi: %d\n", nomorKursi);
    printf("Harga: Rp %.0f\n", jadwal->harga);
    printf("\nKonfirmasi pemesanan? (y/n): ");
    scanf(" %c", &konfirmasi);
    
    if(konfirmasi == 'y' || konfirmasi == 'Y') {
        // Proses pemesanan
        kursi->isTerisi = 1;
        kursi->pelanggan = pelangganSaatIni;
        jadwal->kursiTersedia--;
        
        // Buat string rute
        char rute[MAX_NAMA];
        sprintf(rute, "%s -> %s", jadwal->keberangkatan, jadwal->tujuan);
        
        // Tambah ke riwayat (Stack)
        pushRiwayat(nextIdPemesanan, pelangganSaatIni->id, idJadwal, nomorKursi, 
                     pelangganSaatIni->nama, rute, jadwal->harga, "Dikonfirmasi");
        
        // Tambah ke BST
        rootPohonPemesanan = insertNodePemesanan(rootPohonPemesanan, nextIdPemesanan, pelangganSaatIni->id, 
                                            jadwal, pelangganSaatIni, nomorKursi, "Dikonfirmasi");
        
        printf("\n=== PEMESANAN BERHASIL ===\n");
        printf("ID Pemesanan: %d\n", nextIdPemesanan);
        printf("Tiket telah berhasil dipesan!\n");
        printf("Silakan datang 15 menit sebelum keberangkatan.\n");
        printf("Terima kasih telah memilih Whoosh!\n");
        
        nextIdPemesanan++;
    } else {
        printf("Pemesanan dibatalkan.\n");
    }
}

void lihatPemesananSaya() {
    if(pelangganSaatIni == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    printf("\n=== PEMESANAN SAYA ===\n");
    printf("Pelanggan: %s (ID: %d)\n", pelangganSaatIni->nama, pelangganSaatIni->id);
    printf("===============================================\n");
    
    tampilkanPemesananPelanggan(rootPohonPemesanan, pelangganSaatIni->id);
}

void batalkanPemesananSaya() {
    if(pelangganSaatIni == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    int idPemesanan;
    char alasan[MAX_NAMA];
    
    printf("\n=== BATALKAN PEMESANAN ===\n");
    lihatPemesananSaya(); // Tampilkan pemesanan yang dimiliki pelanggan
    
    printf("\nMasukkan ID Pemesanan yang ingin dibatalkan: ");
    scanf("%d", &idPemesanan);
    getchar(); // Membersihkan newline character
    
    NodePemesanan* pemesanan = searchNodePemesanan(rootPohonPemesanan, idPemesanan);
    
    if(pemesanan == NULL) {
        printf("Pemesanan tidak ditemukan!\n");
        return;
    }
    
    if(pemesanan->idPelanggan != pelangganSaatIni->id) {
        printf("Anda tidak berhak membatalkan pemesanan ini!\n");
        return;
    }
    
    if(strcmp(pemesanan->status, "Dibatalkan") == 0) {
        printf("Pemesanan sudah dibatalkan sebelumnya!\n");
        return;
    }
    
    printf("Alasan pembatalan: ");
    fgets(alasan, MAX_NAMA, stdin);
    alasan[strcspn(alasan, "\n")] = 0;
    
    // Tambah ke antrian pembatalan
    enqueuePembatalan(idPemesanan, pelangganSaatIni->nama, alasan);
    
    // Update status pemesanan
    strcpy(pemesanan->status, "Dibatalkan");
    
    // Kembalikan kursi
    Kursi* kursi = pemesanan->jadwal->daftarKursi;
    while(kursi != NULL) {
        if(kursi->nomorKursi == pemesanan->nomorKursi) {
            kursi->isTerisi = 0;
            kursi->pelanggan = NULL;
            pemesanan->jadwal->kursiTersedia++;
            break;
        }
        kursi = kursi->next;
    }
    
    // Update riwayat
    char rute[MAX_NAMA];
    sprintf(rute, "%s -> %s", pemesanan->jadwal->keberangkatan, pemesanan->jadwal->tujuan);
    pushRiwayat(idPemesanan, pelangganSaatIni->id, pemesanan->jadwal->idJadwal, 
                pemesanan->nomorKursi, pelangganSaatIni->nama, rute, 
                pemesanan->jadwal->harga, "Dibatalkan");
    
    printf("\n=== PEMBATALAN BERHASIL ===\n");
    printf("Pemesanan ID %d telah dibatalkan.\n", idPemesanan);
    printf("Permintaan refund akan diproses dalam 3-5 hari kerja.\n");
}

void lihatRiwayatPemesanan() {
    if(pelangganSaatIni == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    printf("\n=== RIWAYAT PEMESANAN SAYA ===\n");
    printf("Pelanggan: %s\n", pelangganSaatIni->nama);
    printf("===============================================\n");
    
    RiwayatPemesanan* current = stackRiwayat;
    int ditemukan = 0;
    
    while(current != NULL) {
        if(current->idPelanggan == pelangganSaatIni->id) {
            printf("ID Pemesanan: %d\n", current->idPemesanan);
            printf("Rute: %s\n", current->rute);
            printf("Kursi: %d\n", current->nomorKursi);
            printf("Tanggal Pesan: %s\n", current->tanggalPemesanan);
            printf("Status: %s\n", current->status);
            printf("Total: Rp %.0f\n", current->totalHarga);
            printf("-----------------------------------------------\n");
            ditemukan = 1;
        }
        current = current->next;
    }
    
    if(!ditemukan) {
        printf("Belum ada riwayat pemesanan.\n");
    }
}

void tampilkanPetaKursi(JadwalKereta* jadwal) {
    printf("\n=== PETA KURSI ===\n");
    printf("Keterangan: [O] = Kosong, [X] = Terisi\n\n");
    
    int count = 0;
    
    // Urutkan seat berdasarkan nomor untuk tampilan yang benar
    for(int i = 1; i <= jadwal->totalKursi; i++) {
        Kursi* currentKursi = jadwal->daftarKursi;
        while(currentKursi != NULL) {
            if(currentKursi->nomorKursi == i) {
                if(currentKursi->isTerisi) {
                    printf("[X] ");
                } else {
                    printf("[%02d] ", i);
                }
                count++;
                if(count % 4 == 0) printf("    "); // Gang tengah
                if(count % 8 == 0) printf("\n");   // Baris baru
                break;
            }
            currentKursi = currentKursi->next;
        }
    }
    
    if(count % 8 != 0) printf("\n");
    printf("\n");
}

// Fungsi Utilitas
Pelanggan* cariPelangganById(int id) {
    Pelanggan* current = headPelanggan;
    while(current != NULL) {
        if(current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

JadwalKereta* searchJadwalById(int id) {
    JadwalKereta* current = headJadwal;
    while(current != NULL) {
        if(current->idJadwal == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Kursi* buatDaftarKursi(int totalKursi) {
    Kursi* head = NULL;
    
    for(int i = totalKursi; i >= 1; i--) {
        Kursi* newKursi = (Kursi*)malloc(sizeof(Kursi));
        newKursi->nomorKursi = i;
        newKursi->isTerisi = 0;
        newKursi->pelanggan = NULL;
        newKursi->next = head;
        head = newKursi;
    }
    
    return head;
}

void pushRiwayat(int idPemesanan, int idPelanggan, int idJadwal, int nomorKursi, 
                 char* namaPelanggan, char* rute, float harga, char* status) {
    RiwayatPemesanan* newRiwayat = (RiwayatPemesanan*)malloc(sizeof(RiwayatPemesanan));
    newRiwayat->idPemesanan = idPemesanan;
    newRiwayat->idPelanggan = idPelanggan;
    newRiwayat->idJadwal = idJadwal;
    newRiwayat->nomorKursi = nomorKursi;
    strcpy(newRiwayat->namaPelanggan, namaPelanggan);
    strcpy(newRiwayat->rute, rute);
    strcpy(newRiwayat->status, status);
    newRiwayat->totalHarga = harga;
    getTanggalSaatIni(newRiwayat->tanggalPemesanan);
    newRiwayat->next = stackRiwayat;
    stackRiwayat = newRiwayat;
}

void enqueuePembatalan(int idPemesanan, char* namaPelanggan, char* alasan) {
    AntrianPembatalan* newPembatalan = (AntrianPembatalan*)malloc(sizeof(AntrianPembatalan));
    newPembatalan->idPemesanan = idPemesanan;
    strcpy(newPembatalan->namaPelanggan, namaPelanggan);
    strcpy(newPembatalan->alasan, alasan);
    getTanggalSaatIni(newPembatalan->tanggalPermintaan);
    newPembatalan->next = NULL;
    
    if(tailPembatalan == NULL) {
        headPembatalan = tailPembatalan = newPembatalan;
    } else {
        tailPembatalan->next = newPembatalan;
        tailPembatalan = newPembatalan;
    }
}

NodePemesanan* insertNodePemesanan(NodePemesanan* root, int idPemesanan, int idPelanggan, 
                                  JadwalKereta* jadwal, Pelanggan* pelanggan, int nomorKursi, char* status) {
    if(root == NULL) {
        NodePemesanan* newNode = (NodePemesanan*)malloc(sizeof(NodePemesanan));
        newNode->idPemesanan = idPemesanan;
        newNode->idPelanggan = idPelanggan;
        newNode->jadwal = jadwal;
        newNode->pelanggan = pelanggan;
        newNode->nomorKursi = nomorKursi;
        strcpy(newNode->status, status);
        getTanggalSaatIni(newNode->tanggalPemesanan);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    
    if(idPemesanan < root->idPemesanan) {
        root->left = insertNodePemesanan(root->left, idPemesanan, idPelanggan, jadwal, pelanggan, nomorKursi, status);
    } else if(idPemesanan > root->idPemesanan) {
        root->right = insertNodePemesanan(root->right, idPemesanan, idPelanggan, jadwal, pelanggan, nomorKursi, status);
    }
    
    return root;
}

NodePemesanan* searchNodePemesanan(NodePemesanan* root, int idPemesanan) {
    if(root == NULL || root->idPemesanan == idPemesanan) {
        return root;
    }
    
    if(idPemesanan < root->idPemesanan) {
        return searchNodePemesanan(root->left, idPemesanan);
    }
    
    return searchNodePemesanan(root->right, idPemesanan);
}

void tampilkanPemesananPelanggan(NodePemesanan* root, int idPelanggan) {
    if(root == NULL) return;
    
    tampilkanPemesananPelanggan(root->left, idPelanggan);
    
    if(root->idPelanggan == idPelanggan) {
        printf("ID Pemesanan: %d\n", root->idPemesanan);
        printf("Rute: %s -> %s\n", root->jadwal->keberangkatan, root->jadwal->tujuan);
        printf("Tanggal: %s\n", root->jadwal->tanggal);
        printf("Waktu: %s - %s\n", root->jadwal->waktuBerangkat, root->jadwal->waktuTiba);
        printf("Kursi: %d\n", root->nomorKursi);
        printf("Status: %s\n", root->status);
        printf("Harga: Rp %.0f\n", root->jadwal->harga);
        printf("Tanggal Pesan: %s\n", root->tanggalPemesanan);
        printf("-----------------------------------------------\n");
    }
    
    tampilkanPemesananPelanggan(root->right, idPelanggan);
}

void getTanggalSaatIni(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void getWaktuSaatIni(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d:%02d", tm.tm_hour, tm.tm_min);
}