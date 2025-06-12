#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 100
#define MAX_SEATS 100

// Struktur untuk data customer
typedef struct Customer {
    int id;
    char name[MAX_NAME];
    char phone[15];
    char email[MAX_NAME];
    char address[MAX_NAME];
    struct Customer* next;
} Customer;

// Struktur untuk kursi kereta
typedef struct Seat {
    int seatNumber;
    int isOccupied;
    Customer* customer;
    struct Seat* next;
} Seat;

// Struktur untuk jadwal kereta (data statis)
typedef struct Schedule {
    int scheduleId;
    char departure[MAX_NAME];
    char arrival[MAX_NAME];
    char departureTime[10];
    char arrivalTime[10];
    char date[15];
    float price;
    int totalSeats;
    int availableSeats;
    Seat* seatList;
    struct Schedule* next;
} Schedule;

// Struktur untuk riwayat pemesanan customer (Stack)
typedef struct BookingHistory {
    int bookingId;
    int customerId;
    int scheduleId;
    int seatNumber;
    char customerName[MAX_NAME];
    char route[MAX_NAME];
    char bookingDate[20];
    char status[20];
    float totalPrice;
    struct BookingHistory* next;
} BookingHistory;

// Struktur untuk antrian pembatalan (Queue)
typedef struct CancellationQueue {
    int bookingId;
    char customerName[MAX_NAME];
    char reason[MAX_NAME];
    char requestDate[20];
    struct CancellationQueue* next;
} CancellationQueue;

// Tree untuk pencarian cepat pemesanan
typedef struct BookingNode {
    int bookingId;
    int customerId;
    Schedule* schedule;
    Customer* customer;
    int seatNumber;
    char bookingDate[20];
    char status[20];
    struct BookingNode* left;
    struct BookingNode* right;
} BookingNode;

// Global variables
Schedule* scheduleHead = NULL;
Customer* customerHead = NULL;
BookingHistory* historyStack = NULL;
CancellationQueue* cancellationHead = NULL;
CancellationQueue* cancellationTail = NULL;
BookingNode* bookingTreeRoot = NULL;
int nextBookingId = 1001;
int nextCustomerId = 1;

// Function prototypes
void initializeSystem();
void customerMenu();
void registerCustomer();
void loginCustomer();
void displaySchedules();
void searchSchedule();
void bookTicket();
void viewMyBookings();
void cancelMyBooking();
void viewBookingHistory();
void displaySeatMap(Schedule* schedule);
Customer* findCustomerById(int id);
Schedule* findScheduleById(int id);
Seat* createSeatList(int totalSeats);
void pushHistory(int bookingId, int customerId, int scheduleId, int seatNumber, char* customerName, char* route, float price, char* status);
void enqueueCancellation(int bookingId, char* customerName, char* reason);
BookingNode* insertBookingNode(BookingNode* root, int bookingId, int customerId, Schedule* schedule, Customer* customer, int seatNumber, char* status);
BookingNode* searchBookingNode(BookingNode* root, int bookingId);
BookingNode* searchBookingByCustomer(BookingNode* root, int customerId);
void displayCustomerBookings(BookingNode* root, int customerId);
void getCurrentDate(char* buffer);
void getCurrentTime(char* buffer);

// Global variable untuk customer yang sedang login
Customer* currentCustomer = NULL;

int main() {
    initializeSystem();
    customerMenu();
    return 0;
}

void initializeSystem() {
    printf("=== SISTEM PEMESANAN TIKET KERETA WHOOSH ===\n");
    printf("Selamat datang di layanan pemesanan tiket Whoosh!\n");
    printf("Menginisialisasi sistem...\n\n");
    
    // Inisialisasi jadwal kereta (data statis)
    Schedule* schedule1 = (Schedule*)malloc(sizeof(Schedule));
    schedule1->scheduleId = 1;
    strcpy(schedule1->departure, "Halim");
    strcpy(schedule1->arrival, "Padalarang");
    strcpy(schedule1->departureTime, "06:00");
    strcpy(schedule1->arrivalTime, "07:30");
    strcpy(schedule1->date, "2025-06-15");
    schedule1->price = 150000;
    schedule1->totalSeats = 50;
    schedule1->availableSeats = 50;
    schedule1->seatList = createSeatList(50);
    schedule1->next = NULL;
    scheduleHead = schedule1;
    
    Schedule* schedule2 = (Schedule*)malloc(sizeof(Schedule));
    schedule2->scheduleId = 2;
    strcpy(schedule2->departure, "Padalarang");
    strcpy(schedule2->arrival, "Halim");
    strcpy(schedule2->departureTime, "08:00");
    strcpy(schedule2->arrivalTime, "09:30");
    strcpy(schedule2->date, "2025-06-15");
    schedule2->price = 150000;
    schedule2->totalSeats = 50;
    schedule2->availableSeats = 50;
    schedule2->seatList = createSeatList(50);
    schedule2->next = schedule1;
    scheduleHead = schedule2;
    
    Schedule* schedule3 = (Schedule*)malloc(sizeof(Schedule));
    schedule3->scheduleId = 3;
    strcpy(schedule3->departure, "Halim");
    strcpy(schedule3->arrival, "Padalarang");
    strcpy(schedule3->departureTime, "10:00");
    strcpy(schedule3->arrivalTime, "11:30");
    strcpy(schedule3->date, "2025-06-15");
    schedule3->price = 150000;
    schedule3->totalSeats = 50;
    schedule3->availableSeats = 50;
    schedule3->seatList = createSeatList(50);
    schedule3->next = schedule2;
    scheduleHead = schedule3;
    
    Schedule* schedule4 = (Schedule*)malloc(sizeof(Schedule));
    schedule4->scheduleId = 4;
    strcpy(schedule4->departure, "Padalarang");
    strcpy(schedule4->arrival, "Halim");
    strcpy(schedule4->departureTime, "15:00");
    strcpy(schedule4->arrivalTime, "16:30");
    strcpy(schedule4->date, "2025-06-15");
    schedule4->price = 150000;
    schedule4->totalSeats = 50;
    schedule4->availableSeats = 50;
    schedule4->seatList = createSeatList(50);
    schedule4->next = schedule3;
    scheduleHead = schedule4;
    
    printf("Sistem berhasil diinisialisasi!\n");
    printf("Tersedia %d jadwal kereta untuk hari ini.\n\n", 4);
}

void customerMenu() {
    int choice;
    
    while(1) {
        if(currentCustomer == NULL) {
            printf("=== SELAMAT DATANG DI WHOOSH ===\n");
            printf("1. Daftar Akun Baru\n");
            printf("2. Login\n");
            printf("3. Lihat Jadwal Kereta\n");
            printf("4. Cari Jadwal\n");
            printf("0. Keluar\n");
            printf("Pilihan: ");
            scanf("%d", &choice);
            getchar();
            
            switch(choice) {
                case 1:
                    registerCustomer();
                    break;
                case 2:
                    loginCustomer();
                    break;
                case 3:
                    displaySchedules();
                    break;
                case 4:
                    searchSchedule();
                    break;
                case 0:
                    printf("Terima kasih telah menggunakan layanan Whoosh!\n");
                    exit(0);
                default:
                    printf("Pilihan tidak valid!\n");
            }
        } else {
            printf("\n=== MENU CUSTOMER - %s ===\n", currentCustomer->name);
            printf("1. Lihat Jadwal Kereta\n");
            printf("2. Cari Jadwal\n");
            printf("3. Pesan Tiket\n");
            printf("4. Lihat Pemesanan Saya\n");
            printf("5. Batalkan Pemesanan\n");
            printf("6. Riwayat Pemesanan\n");
            printf("7. Logout\n");
            printf("0. Keluar\n");
            printf("Pilihan: ");
            scanf("%d", &choice);
            getchar();
            
            switch(choice) {
                case 1:
                    displaySchedules();
                    break;
                case 2:
                    searchSchedule();
                    break;
                case 3:
                    bookTicket();
                    break;
                case 4:
                    viewMyBookings();
                    break;
                case 5:
                    cancelMyBooking();
                    break;
                case 6:
                    viewBookingHistory();
                    break;
                case 7:
                    currentCustomer = NULL;
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

void registerCustomer() {
    Customer* newCustomer = (Customer*)malloc(sizeof(Customer));
    
    printf("\n=== DAFTAR AKUN BARU ===\n");
    newCustomer->id = nextCustomerId++;
    
    printf("Nama Lengkap: ");
    fgets(newCustomer->name, MAX_NAME, stdin);
    newCustomer->name[strcspn(newCustomer->name, "\n")] = 0;
    
    printf("Nomor Telepon: ");
    fgets(newCustomer->phone, 15, stdin);
    newCustomer->phone[strcspn(newCustomer->phone, "\n")] = 0;
    
    printf("Email: ");
    fgets(newCustomer->email, MAX_NAME, stdin);
    newCustomer->email[strcspn(newCustomer->email, "\n")] = 0;
    
    printf("Alamat: ");
    fgets(newCustomer->address, MAX_NAME, stdin);
    newCustomer->address[strcspn(newCustomer->address, "\n")] = 0;
    
    newCustomer->next = customerHead;
    customerHead = newCustomer;
    
    printf("\n=== PENDAFTARAN BERHASIL ===\n");
    printf("ID Customer: %d\n", newCustomer->id);
    printf("Nama: %s\n", newCustomer->name);
    printf("Silakan login untuk melanjutkan.\n");
}

void loginCustomer() {
    int customerId;
    char phone[15];
    
    printf("\n=== LOGIN CUSTOMER ===\n");
    printf("ID Customer: ");
    scanf("%d", &customerId);
    getchar();
    
    printf("Nomor Telepon: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = 0;
    
    Customer* customer = findCustomerById(customerId);
    
    if(customer != NULL && strcmp(customer->phone, phone) == 0) {
        currentCustomer = customer;
        printf("\n=== LOGIN BERHASIL ===\n");
        printf("Selamat datang, %s!\n", customer->name);
    } else {
        printf("ID Customer atau nomor telepon salah!\n");
    }
}

void displaySchedules() {
    printf("\n=== JADWAL KERETA WHOOSH HARI INI ===\n");
    printf("%-4s %-15s %-15s %-8s %-8s %-12s %-10s %-8s\n", 
           "ID", "Keberangkatan", "Tujuan", "Depart", "Arrive", "Tanggal", "Harga", "Tersedia");
    printf("-----------------------------------------------------------------------------------------\n");
    
    Schedule* current = scheduleHead;
    while(current != NULL) {
        printf("%-4d %-15s %-15s %-8s %-8s %-12s Rp%-8.0f %-8d\n",
               current->scheduleId, current->departure, current->arrival,
               current->departureTime, current->arrivalTime, current->date,
               current->price, current->availableSeats);
        current = current->next;
    }
    
    printf("\nKeterangan: Semua jadwal untuk tanggal %s\n", "2025-06-15");
}

void searchSchedule() {
    char departure[MAX_NAME], arrival[MAX_NAME];
    
    printf("\n=== CARI JADWAL KERETA ===\n");
    printf("Stasiun Keberangkatan: ");
    fgets(departure, MAX_NAME, stdin);
    departure[strcspn(departure, "\n")] = 0;
    
    printf("Stasiun Tujuan: ");
    fgets(arrival, MAX_NAME, stdin);
    arrival[strcspn(arrival, "\n")] = 0;
    
    printf("\n=== HASIL PENCARIAN ===\n");
    printf("Rute: %s -> %s\n", departure, arrival);
    printf("%-4s %-8s %-8s %-12s %-10s %-8s\n", 
           "ID", "Depart", "Arrive", "Tanggal", "Harga", "Tersedia");
    printf("---------------------------------------------------------------\n");
    
    Schedule* current = scheduleHead;
    int found = 0;
    
    while(current != NULL) {
        if(strcasecmp(current->departure, departure) == 0 && 
           strcasecmp(current->arrival, arrival) == 0) {
            printf("%-4d %-8s %-8s %-12s Rp%-8.0f %-8d\n",
                   current->scheduleId, current->departureTime, current->arrivalTime,
                   current->date, current->price, current->availableSeats);
            found = 1;
        }
        current = current->next;
    }
    
    if(!found) {
        printf("Tidak ada jadwal untuk rute tersebut.\n");
    }
}

void bookTicket() {
    if(currentCustomer == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    int scheduleId, seatNumber;
    char confirm;
    
    printf("\n=== PESAN TIKET KERETA ===\n");
    displaySchedules();
    
    printf("\nMasukkan ID Jadwal yang ingin dipesan: ");
    scanf("%d", &scheduleId);
    
    Schedule* schedule = findScheduleById(scheduleId);
    if(schedule == NULL) {
        printf("Jadwal tidak ditemukan!\n");
        return;
    }
    
    if(schedule->availableSeats <= 0) {
        printf("Maaf, tiket sudah habis untuk jadwal ini!\n");
        return;
    }
    
    printf("\n=== DETAIL JADWAL ===\n");
    printf("Rute: %s -> %s\n", schedule->departure, schedule->arrival);
    printf("Tanggal: %s\n", schedule->date);
    printf("Waktu: %s - %s\n", schedule->departureTime, schedule->arrivalTime);
    printf("Harga: Rp %.0f\n", schedule->price);
    printf("Kursi Tersedia: %d\n", schedule->availableSeats);
    
    // Tampilkan peta kursi
    displaySeatMap(schedule);
    
    printf("\nPilih nomor kursi (1-%d): ", schedule->totalSeats);
    scanf("%d", &seatNumber);
    
    if(seatNumber < 1 || seatNumber > schedule->totalSeats) {
        printf("Nomor kursi tidak valid!\n");
        return;
    }
    
    // Cek ketersediaan kursi
    Seat* seat = schedule->seatList;
    while(seat != NULL && seat->seatNumber != seatNumber) {
        seat = seat->next;
    }
    
    if(seat == NULL || seat->isOccupied) {
        printf("Kursi nomor %d sudah terisi!\n", seatNumber);
        return;
    }
    
    // Konfirmasi pemesanan
    printf("\n=== KONFIRMASI PEMESANAN ===\n");
    printf("Nama: %s\n", currentCustomer->name);
    printf("Telepon: %s\n", currentCustomer->phone);
    printf("Rute: %s -> %s\n", schedule->departure, schedule->arrival);
    printf("Tanggal: %s\n", schedule->date);
    printf("Waktu: %s - %s\n", schedule->departureTime, schedule->arrivalTime);
    printf("Kursi: %d\n", seatNumber);
    printf("Harga: Rp %.0f\n", schedule->price);
    printf("\nKonfirmasi pemesanan? (y/n): ");
    scanf(" %c", &confirm);
    
    if(confirm == 'y' || confirm == 'Y') {
        // Proses pemesanan
        seat->isOccupied = 1;
        seat->customer = currentCustomer;
        schedule->availableSeats--;
        
        // Buat route string
        char route[MAX_NAME];
        sprintf(route, "%s -> %s", schedule->departure, schedule->arrival);
        
        // Tambah ke riwayat (Stack)
        pushHistory(nextBookingId, currentCustomer->id, scheduleId, seatNumber, 
                   currentCustomer->name, route, schedule->price, "Confirmed");
        
        // Tambah ke BST
        bookingTreeRoot = insertBookingNode(bookingTreeRoot, nextBookingId, currentCustomer->id, 
                                          schedule, currentCustomer, seatNumber, "Confirmed");
        
        printf("\n=== PEMESANAN BERHASIL ===\n");
        printf("ID Pemesanan: %d\n", nextBookingId);
        printf("Tiket telah berhasil dipesan!\n");
        printf("Silakan datang 15 menit sebelum keberangkatan.\n");
        printf("Terima kasih telah memilih Whoosh!\n");
        
        nextBookingId++;
    } else {
        printf("Pemesanan dibatalkan.\n");
    }
}

void viewMyBookings() {
    if(currentCustomer == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    printf("\n=== PEMESANAN SAYA ===\n");
    printf("Customer: %s (ID: %d)\n", currentCustomer->name, currentCustomer->id);
    printf("===============================================\n");
    
    displayCustomerBookings(bookingTreeRoot, currentCustomer->id);
}

void cancelMyBooking() {
    if(currentCustomer == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    int bookingId;
    char reason[MAX_NAME];
    
    printf("\n=== BATALKAN PEMESANAN ===\n");
    viewMyBookings();
    
    printf("\nMasukkan ID Pemesanan yang ingin dibatalkan: ");
    scanf("%d", &bookingId);
    getchar();
    
    BookingNode* booking = searchBookingNode(bookingTreeRoot, bookingId);
    
    if(booking == NULL) {
        printf("Pemesanan tidak ditemukan!\n");
        return;
    }
    
    if(booking->customerId != currentCustomer->id) {
        printf("Anda tidak berhak membatalkan pemesanan ini!\n");
        return;
    }
    
    if(strcmp(booking->status, "Cancelled") == 0) {
        printf("Pemesanan sudah dibatalkan sebelumnya!\n");
        return;
    }
    
    printf("Alasan pembatalan: ");
    fgets(reason, MAX_NAME, stdin);
    reason[strcspn(reason, "\n")] = 0;
    
    // Tambah ke antrian pembatalan
    enqueueCancellation(bookingId, currentCustomer->name, reason);
    
    // Update status pemesanan
    strcpy(booking->status, "Cancelled");
    
    // Kembalikan kursi
    Seat* seat = booking->schedule->seatList;
    while(seat != NULL) {
        if(seat->seatNumber == booking->seatNumber) {
            seat->isOccupied = 0;
            seat->customer = NULL;
            booking->schedule->availableSeats++;
            break;
        }
        seat = seat->next;
    }
    
    // Update riwayat
    char route[MAX_NAME];
    sprintf(route, "%s -> %s", booking->schedule->departure, booking->schedule->arrival);
    pushHistory(bookingId, currentCustomer->id, booking->schedule->scheduleId, 
               booking->seatNumber, currentCustomer->name, route, 
               booking->schedule->price, "Cancelled");
    
    printf("\n=== PEMBATALAN BERHASIL ===\n");
    printf("Pemesanan ID %d telah dibatalkan.\n", bookingId);
    printf("Permintaan refund akan diproses dalam 3-5 hari kerja.\n");
}

void viewBookingHistory() {
    if(currentCustomer == NULL) {
        printf("Anda harus login terlebih dahulu!\n");
        return;
    }
    
    printf("\n=== RIWAYAT PEMESANAN SAYA ===\n");
    printf("Customer: %s\n", currentCustomer->name);
    printf("===============================================\n");
    
    BookingHistory* current = historyStack;
    int found = 0;
    
    while(current != NULL) {
        if(current->customerId == currentCustomer->id) {
            printf("ID Pemesanan: %d\n", current->bookingId);
            printf("Rute: %s\n", current->route);
            printf("Kursi: %d\n", current->seatNumber);
            printf("Tanggal Pesan: %s\n", current->bookingDate);
            printf("Status: %s\n", current->status);
            printf("Total: Rp %.0f\n", current->totalPrice);
            printf("-----------------------------------------------\n");
            found = 1;
        }
        current = current->next;
    }
    
    if(!found) {
        printf("Belum ada riwayat pemesanan.\n");
    }
}

void displaySeatMap(Schedule* schedule) {
    printf("\n=== PETA KURSI ===\n");
    printf("Keterangan: [O] = Kosong, [X] = Terisi\n\n");
    
    int count = 0;
    Seat* seat = schedule->seatList;
    
    // Urutkan seat berdasarkan nomor untuk tampilan yang benar
    for(int i = 1; i <= schedule->totalSeats; i++) {
        Seat* currentSeat = schedule->seatList;
        while(currentSeat != NULL) {
            if(currentSeat->seatNumber == i) {
                if(currentSeat->isOccupied) {
                    printf("[X] ");
                } else {
                    printf("[%02d] ", i);
                }
                count++;
                if(count % 4 == 0) printf("    "); // Gang tengah
                if(count % 8 == 0) printf("\n");   // Baris baru
                break;
            }
            currentSeat = currentSeat->next;
        }
    }
    
    if(count % 8 != 0) printf("\n");
    printf("\n");
}

// Utility Functions
Customer* findCustomerById(int id) {
    Customer* current = customerHead;
    while(current != NULL) {
        if(current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Schedule* findScheduleById(int id) {
    Schedule* current = scheduleHead;
    while(current != NULL) {
        if(current->scheduleId == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Seat* createSeatList(int totalSeats) {
    Seat* head = NULL;
    
    for(int i = totalSeats; i >= 1; i--) {
        Seat* newSeat = (Seat*)malloc(sizeof(Seat));
        newSeat->seatNumber = i;
        newSeat->isOccupied = 0;
        newSeat->customer = NULL;
        newSeat->next = head;
        head = newSeat;
    }
    
    return head;
}

void pushHistory(int bookingId, int customerId, int scheduleId, int seatNumber, 
                char* customerName, char* route, float price, char* status) {
    BookingHistory* newHistory = (BookingHistory*)malloc(sizeof(BookingHistory));
    newHistory->bookingId = bookingId;
    newHistory->customerId = customerId;
    newHistory->scheduleId = scheduleId;
    newHistory->seatNumber = seatNumber;
    strcpy(newHistory->customerName, customerName);
    strcpy(newHistory->route, route);
    strcpy(newHistory->status, status);
    newHistory->totalPrice = price;
    getCurrentDate(newHistory->bookingDate);
    newHistory->next = historyStack;
    historyStack = newHistory;
}

void enqueueCancellation(int bookingId, char* customerName, char* reason) {
    CancellationQueue* newCancellation = (CancellationQueue*)malloc(sizeof(CancellationQueue));
    newCancellation->bookingId = bookingId;
    strcpy(newCancellation->customerName, customerName);
    strcpy(newCancellation->reason, reason);
    getCurrentDate(newCancellation->requestDate);
    newCancellation->next = NULL;
    
    if(cancellationTail == NULL) {
        cancellationHead = cancellationTail = newCancellation;
    } else {
        cancellationTail->next = newCancellation;
        cancellationTail = newCancellation;
    }
}

BookingNode* insertBookingNode(BookingNode* root, int bookingId, int customerId, 
                              Schedule* schedule, Customer* customer, int seatNumber, char* status) {
    if(root == NULL) {
        BookingNode* newNode = (BookingNode*)malloc(sizeof(BookingNode));
        newNode->bookingId = bookingId;
        newNode->customerId = customerId;
        newNode->schedule = schedule;
        newNode->customer = customer;
        newNode->seatNumber = seatNumber;
        strcpy(newNode->status, status);
        getCurrentDate(newNode->bookingDate);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    
    if(bookingId < root->bookingId) {
        root->left = insertBookingNode(root->left, bookingId, customerId, schedule, customer, seatNumber, status);
    } else if(bookingId > root->bookingId) {
        root->right = insertBookingNode(root->right, bookingId, customerId, schedule, customer, seatNumber, status);
    }
    
    return root;
}

BookingNode* searchBookingNode(BookingNode* root, int bookingId) {
    if(root == NULL || root->bookingId == bookingId) {
        return root;
    }
    
    if(bookingId < root->bookingId) {
        return searchBookingNode(root->left, bookingId);
    }
    
    return searchBookingNode(root->right, bookingId);
}

void displayCustomerBookings(BookingNode* root, int customerId) {
    if(root == NULL) return;
    
    displayCustomerBookings(root->left, customerId);
    
    if(root->customerId == customerId) {
        printf("ID Pemesanan: %d\n", root->bookingId);
        printf("Rute: %s -> %s\n", root->schedule->departure, root->schedule->arrival);
        printf("Tanggal: %s\n", root->schedule->date);
        printf("Waktu: %s - %s\n", root->schedule->departureTime, root->schedule->arrivalTime);
        printf("Kursi: %d\n", root->seatNumber);
        printf("Status: %s\n", root->status);
        printf("Harga: Rp %.0f\n", root->schedule->price);
        printf("Tanggal Pesan: %s\n", root->bookingDate);
        printf("-----------------------------------------------\n");
    }
    
    displayCustomerBookings(root->right, customerId);
}

void getCurrentDate(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void getCurrentTime(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d:%02d", tm.tm_hour, tm.tm_min);
}