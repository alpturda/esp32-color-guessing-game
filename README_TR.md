# ESP32 Renk Tahmin Oyunu

ESP32 Renk Tahmin Oyunu; ESP32 mikrodenetleyici, ST7789 TFT ekran, rotary encoder ve özel tasarlanmış 3D baskı kasa kullanılarak geliştirilmiş gömülü sistem tabanlı bir renk tahmin oyunudur.

Bu proje, renk kodu tahmin etme mantığına dayanmaktadır. Sistem oyun başlangıcında 8 renkli palet içerisinden rastgele 4 renkli gizli bir kod oluşturur. Kullanıcı, rotary encoder yardımıyla renk seçimi yapar ve encoder butonuna basarak seçimini onaylar.

Her tahmin TFT ekran üzerinde gösterilir. Sistem, kullanıcının tahminini gizli kod ile karşılaştırır ve doğru renk / doğru konum bilgisine göre görsel geri bildirim verir.

## Çalışan Prototip

![Çalışan Prototip](images/working-prototype.jpg)

## Proje Özellikleri

- ESP32 tabanlı gömülü oyun sistemi
- ST7789 240x320 TFT ekran arayüzü
- Rotary encoder ile kullanıcı kontrolü
- 8 renkli seçim paleti
- 4 haneli gizli renk kodu
- Maksimum 8 deneme hakkı
- Ekranda tahmin geçmişi gösterimi
- Renkli geri bildirim sistemi
- State machine tabanlı oyun akışı
- Özel tasarlanmış 3D baskı kasa
- 3.7V Li-ion pil ile taşınabilir kullanım

## Kullanılan Donanımlar

- ESP32 geliştirme kartı
- ST7789 240x320 TFT ekran modülü
- Rotary encoder
- Back/delete butonu
- 3.7V Li-ion pil
- Li-ion pil şarj modülü
- 3.3V voltaj regülatörü
- Jumper kablolar
- Özel 3D baskı kasa

## Güç Sistemi

Proje 3.7V Li-ion pil ile çalışacak şekilde tasarlanmıştır.

Pilin şarj edilebilmesi için bir Li-ion şarj modülü kullanılmıştır. ESP32 ve devre için kararlı 3.3V besleme sağlamak amacıyla voltaj regülatörü kullanılmıştır.

### Güç Sistemi Bileşenleri

- 3.7V Li-ion pil
- Li-ion pil şarj modülü
- 3.3V voltaj regülatörü

> Not: Güç bağlantıları kullanılan ESP32 geliştirme kartına, şarj modülüne ve regülatöre göre değişiklik gösterebilir.

## Pin Bağlantıları

| Bileşen | ESP32 Pin |
|---|---|
| TFT CS | GPIO 5 |
| TFT DC | GPIO 2 |
| TFT RST | GPIO 4 |
| TFT SCK / SCL | GPIO 18 |
| TFT MOSI / SDA | GPIO 23 |
| Encoder A / CLK | GPIO 32 |
| Encoder B / DT | GPIO 33 |
| Encoder Push / SW | GPIO 25 |
| Back Button | GPIO 26 |

> Not: ST7789 ekran modüllerinde pin isimleri modül versiyonuna göre değişebilir. Bazı modüllerde SPI pinleri SCL/SDA olarak, bazı modüllerde ise SCK/MOSI olarak adlandırılabilir.

## Bağlantı Şeması

![Modül Bağlantı Şeması](schematics/module-style-wiring-diagram.png)

## Oyun Mantığı

Sistem oyun başında rastgele 4 renkli gizli bir kod oluşturur.

Kullanıcı, rotary encoder ile renk seçimi yapar ve encoder butonuna basarak seçimini onaylar.

Her 4 renkli tahmin sonrasında sistem tahmini gizli kod ile karşılaştırır:

- Yeşil: Doğru renk ve doğru konum
- Sarı: Doğru renk fakat yanlış konum
- Beyaz / Boş: Eşleşme yok

Kullanıcı 8 deneme içinde tüm renkleri doğru konumda bulursa oyunu kazanır. Eğer 8 deneme sonunda doğru kod bulunamazsa sistem gizli kodu ekranda gösterir.

## 3D Baskı Kasa

Bu proje için özel bir kasa tasarlanmış ve 3D yazıcı ile üretilmiştir.

Kasa; ESP32, ST7789 TFT ekran, rotary encoder, back/delete butonu, güç anahtarı, pil, şarj modülü ve voltaj regülatörünü kompakt bir prototip yapısı içinde tutacak şekilde tasarlanmıştır.

### 3D Model Genel Görünümü

![3D Model Genel Görünümü](images/3d-model-overview.png)

### Kasa Ön Görünümü

![Kasa Ön Görünümü](images/enclosure-front-view.jpg)

### Kasa Yan Görünümü

![Kasa Yan Görünümü](images/enclosure-side-view.jpg)

## 3D Model Dosyaları

Kasa iki ana parçadan oluşmaktadır:

- `case` – ana kasa gövdesi
- `cover` – ön kapak / üst panel

Her iki parça için STL ve STEP dosyaları paylaşılmıştır.

### Case

- STL: `3d-models/case/stl-files/case.stl`
- STEP: `3d-models/case/step-files/case.step`

### Cover

- STL: `3d-models/cover/stl-files/cover.stl`
- STEP: `3d-models/cover/step-files/cover.step`

STL dosyaları 3D baskı için paylaşılmıştır.

STEP dosyaları ise CAD düzenleme ve model üzerinde değişiklik yapılabilmesi için paylaşılmıştır.

## Kullanılan Teknolojiler

- ESP32
- Arduino IDE
- C/C++
- Adafruit GFX Library
- Adafruit ST7789 Library
- SPI haberleşme
- Rotary encoder girişi
- Gömülü sistem arayüz tasarımı
- 3D modelleme
- 3D baskı

## Proje Yapısı

```text
esp32-color-guessing-game
├── 3d-models
│   ├── case
│   │   ├── step-files
│   │   │   └── case.step
│   │   └── stl-files
│   │       └── case.stl
│   │
│   └── cover
│       ├── step-files
│       │   └── cover.step
│       └── stl-files
│           └── cover.stl
│
├── images
│   ├── 3d-model-overview.png
│   ├── enclosure-front-view.jpg
│   ├── enclosure-side-view.jpg
│   └── working-prototype.jpg
│
├── schematics
│   └── module-style-wiring-diagram.png
│
├── src
│   └── esp32-color-guessing-game
│       └── esp32-color-guessing-game.ino
│
├── LICENSE
├── README.md
└── README_TR.md
