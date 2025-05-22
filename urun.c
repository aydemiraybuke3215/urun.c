#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "product.bat"

struct Product
{
    int id;
    char name[20];
    float price;
    int stock;
    int isDeleted;
};

void menu();
void addProducts();
void updateProduct();
void showProducts();
void deleteProduct();

int main(void)
{
    printf("=== Urun Yonetim Sistemine Hosgeldiniz ===\n");

    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL)
    {
        file = fopen(FILE_NAME, "wb");
        if (file == NULL)
        {
            printf("Veri dosyasi olusturulamadi! Program sonlandiriliyor.\n");
            return 1;
        }
        fclose(file);
        printf("Veri dosyasi olusturuldu.\n");
    }
    else
    {
        fclose(file);
        printf("Veri dosyasi bulundu ve yüklendi.\n");
    }

    while (1)
    {
        menu();
    }

}

void menu()
{
    int choice;

    printf("\n---URUN YONETIMI---\n");
    printf("1. Urun Ekleme\n");
    printf("2. Stok Guncelleme\n");
    printf("3. Urunleri Goster\n");
    printf("4. Urun Silme\n");
    printf("5. Cikis\n");
    printf("Secim Yapiniz: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            addProducts();
            break;
        case 2:
            updateProduct();
            break;
        case 3:
            showProducts();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            printf("Cikis yapiliyor... Tesekkurler!\n");
            exit(0);
            break;
        default:
            printf("Gecersiz secim! Lutfen tekrar deneyin.\n");
    }
}

void addProducts()
{
    struct Product p;
    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL)
    {
        // File does not exist, create new file
        file = fopen(FILE_NAME, "wb+");
        if (file == NULL)
        {
            printf("Dosya acilamadi!\n");
            return;
        }
    }

    struct Product temp;
    int foundDeletedSlot = 0;
    long pos;

    while (fread(&temp, sizeof(struct Product), 1, file) == 1)
    {
        if (temp.isDeleted == 1)
        {
            foundDeletedSlot = 1;
            pos = ftell(file) - sizeof(struct Product);
            break;
        }
    }

    printf("Urun ID: ");
    scanf("%d", &p.id);
    printf("Urun Adi: ");
    scanf("%s", p.name);
    printf("Urun Fiyati: ");
    scanf("%f", &p.price);
    printf("Urun Miktari: ");
    scanf("%d", &p.stock);
    p.isDeleted = 0; // active

    if (foundDeletedSlot)
    {

        fseek(file, pos, SEEK_SET);
        fwrite(&p, sizeof(struct Product), 1, file);
        printf("Urun basariyla eklendi (silinmis kayit üzerine).\n");
    }
    else
    {

        fseek(file, 0, SEEK_END);
        fwrite(&p, sizeof(struct Product), 1, file);
        printf("Urun basariyla eklendi (yeni kayit).\n");
    }

    fclose(file);
}

void updateProduct()
{
    struct Product p;
    int id, newStock;

    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    printf("Guncellenmek istenen Urun ID'sini girin: ");
    scanf("%d", &id);

    long pos = (id - 1) * sizeof(struct Product);
    fseek(file, pos, SEEK_SET);

    if (fread(&p, sizeof(struct Product), 1, file) != 1 || p.isDeleted == 1)
    {
        printf("Urun bulunamadi veya silinmis.\n");
        fclose(file);
        return;
    }

    printf("Mevcut stok miktari: %d\n", p.stock);
    printf("Yeni stok miktari: ");
    scanf("%d", &newStock);

    p.stock = newStock;

    fseek(file, pos, SEEK_SET);
    fwrite(&p, sizeof(struct Product), 1, file);

    fclose(file);
    printf("Stok basariyla guncellendi.\n");
}

void showProducts()
{
    struct Product p;
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL)
    {
        printf("Dosya bulunamadi veya acilamadi.\n");
        return;
    }

    printf("ID\tUrun Adi\tFiyat\tStok\n");
    printf("-------------------------------------\n");

    while (fread(&p, sizeof(struct Product), 1, file) == 1)
    {
        if (p.isDeleted == 0)
        {
            printf("%d\t%s\t\t%.2f\t%d\n", p.id, p.name, p.price, p.stock);
        }
    }

    fclose(file);
}

void deleteProduct()
{
    struct Product p;
    int id;

    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    printf("Silinecek Urun ID'sini girin: ");
    scanf("%d", &id);

    long pos = (id - 1) * sizeof(struct Product);
    fseek(file, pos, SEEK_SET);

    if (fread(&p, sizeof(struct Product), 1, file) != 1)
    {
        printf("Urun bulunamadi!\n");
        fclose(file);
        return;
    }

    if (p.isDeleted == 1)
    {
        printf("Urun zaten silinmis.\n");
        fclose(file);
        return;
    }

    p.isDeleted = 1;

    fseek(file, pos, SEEK_SET);
    fwrite(&p, sizeof(struct Product), 1, file);

    fclose(file);
    printf("Urun basariyla silindi.\n");
}
