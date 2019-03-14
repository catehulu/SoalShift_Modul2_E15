# SoalShift_Modul2_E15

### NOMOR 5

Untuk nomor 5, penyelesaian dari masalah tersebut adalah membuat daemon yang bekerja di direktori "home/[user]/log". Pastinya sebelum melakukan eksekusi daemon ini, dipastikan untuk membuat folder log sebelumnya. Dalam proses daemon, kita akan mengambil tanggal pada saat daemon dijalankan untuk nama folder yang telah ditentukan formatnya yaitu "dd:MM:yyyy-hh:mm" sehingga kita dapatkan waktu dengan cara sebagai berikut:

```
// Atur waktu
setlocale(LC_ALL,"");
t = time(NULL);
timeptr = localtime(&t);
```

Didalam potongan code tersebut, terdapat setlocale yang artinya program akan mengambil waktu lokal dimana sebuah proses tersebut berjalan. Lalu variabel t merupakan variabel yang menyimpan time saat program dijalankan dan timeptr adalah struct tm yang merupakan built in dari library time.h dimana dalam struct tersebut dapat mengambil bagian bagian dalam waktu tersebut. Pada struct tm terdapat struktur sebagai berikut.

```
struct tm {
int tm_sec;         /* seconds,  range 0 to 59          */
int tm_min;         /* minutes, range 0 to 59           */
int tm_hour;        /* hours, range 0 to 23             */
int tm_mday;        /* day of the month, range 1 to 31  */
int tm_mon;         /* month, range 0 to 11             */
int tm_year;        /* The number of years since 1900   */
int tm_wday;        /* day of the week, range 0 to 6    */
int tm_yday;        /* day in the year, range 0 to 365  */
int tm_isdst;       /* daylight saving time             */	
};
```

Lalu setelah mendapatkan waktu pada saat daemon dijalankan(pada saat pembuatan folder waktu yang digunakan adalah 30 menit), maka langkah selanjutnya yang harus dijalankan adalah membuat nama folder sesuai waktu tersebut. Karena nama folder tersebut merupakan string maka diperlukan pengubahan waktu yang didapatkan menjadi string dengan cara sebagai berikut

```
strftime(buf,sizeof(buf), "%d:%m:%Y-%H:%M", timeptr);
strcpy(path, buf);
```

Dalam potongan code diatas terdapat fungsi strftime yang merupakan built-in dari library time.h dimana fungsi ini mengubah sebuah format waktu menjadi string dengan format yang telah ditentukan. Format pada permasalahan ini dapat diambil dengan format "%d:%m:%Y-%H:%M" dimana %d mengambil tanggal, %m mengambil bulan, %Y mengambil tahun, %H mengambil jam dalam format 24 jam, dan juga %M mengambil menit dari format time. Lalu langkah selanjutnya adalah membuat folder sesuai nama folder yang telah didapat sebelumnya. Disini menggunakan mkdir dari built-in c.

```
mkdir(path, ACCESSPERMS);
increment_file=1;
chdir(path);
```

Setelah pembuatan folder selesai maka increment file yang merupakan angka yang akan digunakan dalam pembuatan nama file "log#.log" diubah kembali menjadi satu karena adanya folder yang baru.Selanjutnya proses pembuatan file untuk setiap menitnya adalah sebagai berikut.

```
// Membuat path dari file yang akan dibuat
char name_of_file[100];
sprintf(name_of_file, "log%d.log", increment_file);

// Membuat parameter touch
char *param_cp[]={"cp", "/var/log/syslog", name_of_file, NULL};
execv("/bin/cp", param_cp);
```

Dimana name_of_file merupakan nama file yang akan dibuat sedangkan param_cp adalah parameter untuk proses copy dari syslog. Dalam pembuatan nama file terdapat fungsi sprintf dimana fungsi tersebut mengisi string dengan format yang ditentukan setelahnya. sprintf ini berjalan seperti printf pada c akan tetapi perbedaannya adalah printf dikeluarkan pada terminal atau program yang berjalan sedangkan sprintf hasil dari printf dimasukkan kedalam string variabel. Lalu membuat parameter untuk proses copy dan jalankan dengan execv disertai dengan direktori /bin/cp. Setelah proses tersebut selesai maka incrementkan variabel increment_file.
