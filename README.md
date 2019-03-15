# SoalShift_Modul2_E15

# Nomor 1
Untuk nomor 1 soal meminta untuk membuat merubah semua file yang
berekstensi .png menjadi _grey.png. Di soal diberitahu bahwa file yang akan dirubah sangat banyak, tetapi tidak diberitahu apakah file tersebut diberikan secara sekaligus ataupun secara berkala, tetapi diasumsikan file diberikan secara berkala.

Untuk soal 1 program utama dibagi menjadi 2, yang membaca file dan yang merubah file.

untuk bagian yang membaca file digunakan library dirent. dirent akan membuka directory dan dijadikan working directory.
```
...
directory = opendir(".");
    if (directory) {
        while ((dir = readdir(directory)) != NULL) {
            filename = dir->d_name;
...
```

variabel dir adalah struct dari dir yang dijadikan pointer untuk file yang sedang dibaca. Dari struct tersebut terdapat elemen nama yang dimasukkan ke variabel name. Setelah itu dicek apakah name berekstensi .png atau tidak. Jika iya, maka proses akan dilanjutkan
```
...
if (!tmp || tmp == filename) {
                continue;
            }
            if(strcmp(strrchr(filename,'.'),ext) == 0) {
...
```

Pertama-tama direktori tujuan dimasukkan ke variabel folder. Setelah itu dari nama asli file akan diambil nama saja. Hal ini dilakukan dengan menggunakan [strncpy()](https://www.tutorialspoint.com/c_standard_library/c_function_strncpy.htm). Karena file yang dihandle hanyalah png, maka tiap file dipastikan 4 karakter terakhir adalah .png, maka yang diambil sepanjang length-4.

Setelah itu _grey.png ditambahkan kebelakang variabel nama tanpa ext. Untuk memindahkan file ke direktori tujuan dapat digunakan [rename()](https://linux.die.net/man/3/rename)

```
strcpy(folder,"/home/catehulu/modul2/gambar/");
                lenght = strlen(filename) - 4;
                // printf("fname %s\nlen %d\n", filename,lenght);
                strncpy(newname, filename, lenght);
                // printf("new %s\n", newname);
                strcat(newname, "_grey.png");
                // printf("new grey %s\n", newname);
                strcat(folder, newname);
                // printf("dir %s\n", folder);
                rename(filename,folder);
                memset(newname, '\0', sizeof(newname));
                memset(folder, '\0', sizeof(folder));
```

dan program tersebut dijadikan daemon, dan proses dilakukan setiap 5 detik.

# Nomor 2

Soal nomor 2 meminta untuk program C yang menghapus file "elen.ku" pada direktori "hatiku" jika owner dan dan groupnya "www-data". Sebelum dihapus, permission diganti menjadi 777

Pertama-tama dicek dengan [stat()](https://link) id dari owner dan groupnya. Setelah itu dari library [pwd.h](https://link) dan [grp.h](https://link) terdapat struct yang dapat mereturn data dari id tersebut. Dari struct itu dicek namanya apakah "www-data".
```
...
if(stat(file,&check) == 0){
        owner = getpwuid(check.st_uid);
        group = getgrgid(check.st_gid);
        // printf("%s %s\n", user->pw_name, group->gr_name);
        if(strcmp(owner->pw_name,group->gr_name) == 0 && strcmp(group->gr_name,name) == 0){
            // printf("right");
            // printf("%d\n",i);
            chmod(file,i);
            remove(file);
        }
    }
...
```

permissionnya dirubah dengan chmod, tetapi agar permission dapat diganti maka file harus dijalankan sebagai sudo. 

Karena dicek setiap 3 detik, maka didaemon disleep selama 3 detik

# Nomor 3

Pertama hal yang harus dilakukan adalah mengekstrak file .zip dengan membuat process terlebih dahulu.

```
char *param_unzip[]={"unzip", "campur2.zip", NULL};
execv("/usr/bin/unzip", param_unzip);
```

Kedua, membuat file bernama daftar.txt dengan command touch. Sebenarnya proses ini tidak bergantung pada proses ekstrak file .zip sehingga menggunakan wait atau tidak sama saja.

```
char *param_touch[]={"touch",  "daftar.txt", NULL};
execv("/usr/bin/touch", param_touch);
```

Ketiga, menggunakan fungsi popen() dimana fungsi ini merupakan pipe dari hasil sebuah command ke dalam sebuah file pointer. Setelah itu, file pointer tersebut dibaca dengan bantuan variabel string. Untuk setiap barisnya, lakukan print ke dalam file daftar.txt(yang sebelumnya telah dibuka dengan fopen).

```
FILE *rd, *out;
int MAX_CHAR = 50;
char *result = malloc( 51 * sizeof(char) );
out= fopen("daftar.txt", "w");
rd = popen("ls campur2/*.txt | awk 'BEGIN{FS=\"/\"}{print $2}'", "r"); // Pipe C dengan command di kernel

while(fgets(result, MAX_CHAR, rd)){
    fprintf(out, "%s", result);
}

fclose(out);
pclose(rd);

```

Terakhir, karena file daftar.txt harus dapat dibuka maka permission dari file daftar.txt haruslah diubah kedalam mode 666 yang artinya dapat dibaca dan juga ditulis.

 ```
char *param_chmod[]={"chmod", "666", "daftar.txt", NULL};
execv("/bin/chmod", param_chmod);
 ```

# Nomor 4

Untuk nomor 4, penyelesaian masalah ini adalah mengatur waktu akses terakhir, lalu mengecek apakah dalam 30 detik sebelumnya file tersebut terbuka atau tidak.

```
struct stat check;
if (stat("makan_enak.txt", &check) == -1) {
    printf("salah!");
}

time_t t1;
time_t t2;
struct tm *timeptr;

t1 = time(NULL);
t2 = check.st_atime;
```

Penyelesaian pertama adalah mengecek status dari file makan_enak.txt. Untuk penyelesaian ini menggunakan struct stat check, dimana struct ini terdapat status dari file yang akan dicek yaitu makan_enak.txt. Lalu panggil perintah stat("makan_enak.txt", &check) sehingga semua status file makan enak akan masuk kedalam struct. Lalu pembuatan variabel time dimana t1 adalah waktu sekarang sedangkan t2 adalah waktu yang dikonversi dari check yang diambil access timenya(atime).

```
if((int)difftime(t1, t2)<=30)
```

Potongan codingan diatas merupakan fungsi yang mereturnkan selisih dari 2 range waktu. Secara default, difftime mereturnkan selisih dalam satuan waktu dan variabel double. Disini, digunakan bahwa t1 adalah waktu sekarang sedangkan t2 adalah waktu akses terakhir dari makan_enak.txt

Untuk proses pembuatan file, dapat diselesaikan dengan proses yang mengeksekusi touch.

```
char file_name[100];
sprintf(file_name, "makan_sehat%d.txt", file);
char *param_touch[] = {"touch", file_name, NULL};
execv("/usr/bin/touch", param_touch);
```

Terdapat 2 variabel array of char(string) dimana file_name adalah variabel yang menyimpan nama file yang akan dibuat. Pembuatan nama file, dilakukan dengan fungsi sprintf(built-in c) dimana variabel file_name akan menyimpan string makan_sehat#.txt dimana # akan direplace dengan sebuah variabel file yang telah dideklarasi di global. Sedangkan string yang lain, yaitu param_touch adalah sebuah parameter dimana command touch yang ada dimasukkan kedalam variabel ini, dan akan dimasukkan kedalam fungsi execv yang akan dieksekusi dengan direktori /usr/bin/touch. Setelah proses ini dijalankan, maka incrementkan variabel file tersebut. Lalu sleep selama 5 detik.

# Nomor 5

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
