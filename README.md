# KenKen Solver dengan Backtracking dan Constraint Propagation

Program ini adalah implementasi solver untuk puzzle KenKen menggunakan algoritma backtracking dengan optimasi constraint propagation. Dibuat untuk menyelesaikan tugas makalah mata kuliah Strategi Algoritma.

## Apa itu KenKen?

KenKen adalah puzzle angka yang mirip Sudoku, tapi dengan twist tambahan berupa operasi matematika. Aturan mainnya:

1. **Isi grid NxN** dengan angka 1 sampai N
2. **Setiap baris dan kolom** harus berisi angka unik (seperti Sudoku)
3. **Cage** (kotak dengan garis tebal) harus memenuhi operasi matematika yang ditentukan
4. **Operasi** bisa berupa penjumlahan (+), pengurangan (-), perkalian (*), pembagian (/), atau sama dengan (=)

Contoh: Cage dengan "6+" berarti sel-sel di dalamnya harus dijumlahkan menghasilkan 6.

## Fitur Program

- ✅ **Input interaktif** - masukkan puzzle langsung dari terminal
- ✅ **Input dari file** - baca puzzle dari file .txt  
- ✅ **Constraint Propagation** - bisa diaktifkan/dinonaktifkan untuk perbandingan performa
- ✅ **Timing detail** - pengukuran waktu dengan presisi tinggi (6 digit decimal)
- ✅ **Statistik lengkap** - jumlah backtrack dan constraint checks
- ✅ **Format input sederhana** - format yang mudah dipahami

## Cara Kompilasi

```bash
g++ -o kenken_solver src/main.cpp
```

## Cara Menjalankan

```bash
./kenken_solver
```

Program akan menampilkan menu pilihan:
1. **Interactive input** - ketik puzzle manual
2. **Read from file** - baca dari file .txt

## Format Input File

File input berisi:
- Baris pertama: ukuran puzzle (misal: 4)
- Baris selanjutnya: constraint dalam format `target operation cell1 cell2 ...`

Contoh file `puzzle_4x4.txt`:
```
4
3 + 11 12
6 * 13 23  
4 = 14
2 = 21
1 = 22
3 = 24
```

**Penjelasan format cell:**
- `11` = baris 1, kolom 1
- `23` = baris 2, kolom 3
- dst.

## Contoh Penggunaan

### 1. Menjalankan dari file:
```bash
./kenken_solver
Choose mode: 2
Enter filename: puzzle_4x4.txt
Enable Constraint Propagation? (y/n): y
```

### 2. Input manual:
```bash
./kenken_solver
Choose mode: 1
Enter puzzle size: 3
Enter constraints:
4 + 11 12
3 = 13
(tekan Enter untuk selesai)
```

## Perbandingan Performa

Program ini bisa membandingkan performa dengan/tanpa constraint propagation:

**Dengan Constraint Propagation:**
- Deteksi konflik lebih awal
- Lebih sedikit backtrack
- Biasanya lebih cepat

**Tanpa Constraint Propagation:**
- Cek constraint hanya di akhir
- Lebih banyak backtrack
- Biasanya lebih lambat

Contoh output perbandingan:
```
=== Dengan CP ===
Backtracks: 53
Constraint checks: 97
Time: 0.123456 ms

=== Tanpa CP ===  
Backtracks: 3818
Constraint checks: 562
Time: 1.655000 ms
```

## File Test Cases

Repository ini sudah include beberapa test case:
- `3.txt` - puzzle 3x3 sederhana
- `4.txt` - puzzle 4x4 dengan operasi beragam

## Struktur Project

```
kenken_solver/
├── src/
│   ├── kenken_solver.cpp    # Class utama solver
│   └── main.cpp             # Program utama
├── test/                    # File-file puzzle test
└── README.md                # Dokumentasi ini
```

## Algoritma yang Digunakan

1. **Backtracking** - mencoba semua kemungkinan secara sistematis
2. **Constraint Propagation** - cek constraint setiap kali isi cell (opsional)
3. **Forward Checking** - validasi baris/kolom sebelum lanjut

## Author

| Name | NIM |
|------|------|
| Muhammad Timur Kanigara | 13523055 |
