#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

#define MAX_LEN 256

void clearscr() {
	system("@cls||clear");
}


size_t write_to_file(char *fpath, char *data) {
	FILE *f = fopen(fpath, "w");
	if (f == NULL) {
		printf("Ошибка создания файла %s", fpath);
		return -1;
	} else {
		fprintf(f, "%s", data);
		fclose(f);
	}
	return 0;
}

size_t read_from_file(char *fpath, char *data) {
	FILE *f = fopen(fpath, "r");
	if (f == NULL) {
		printf("Ошибка открытия файла %s", fpath);
		return -1;
	} else {
		fscanf(f, "%s", data);
		fclose(f);
	}
	return 0;
}

size_t read_key(size_t *key) {
	size_t key_len = 0;
	printf("Введите длину ключа: ");
        scanf("%zd", &key_len);
	if (key_len > MAX_LEN) {
		printf("Извините, но такая длиная ключа не поддерживается!\n");
	} else {
		for (size_t i = 0; i < key_len; ++i) {
			printf("Введите %zd элемент ключа: ", i);
			scanf("%zd", &key[i]);
			if (key[i] > key_len) {
				printf("Выбран несуществующий столбец! Вы не получите результата.\n");
			}
		}
	}
	return key_len;
}

size_t encrypt_routine() {
	clearscr();
	char *plaintext = (char *)malloc(sizeof(char) * MAX_LEN);
	size_t *key = (size_t *)malloc(sizeof(size_t) * MAX_LEN);
	memset(plaintext, 0, MAX_LEN);
	memset(key, 0, MAX_LEN);
	size_t key_len = read_key(key);
	if (key_len == 0) {
		return -1;
	}
	printf("Полученный ключ:\n{ ");
	for (size_t i = 0; i < key_len; ++i) {
		printf("%zd ", key[i]);
	}
	printf("}\n");
	printf("Открытый текст будет прочитан из файла с именем plaintext!\n");
	if (read_from_file("plaintext", plaintext) != 0) {
		return -1;
	}
	size_t p_len = strlen(plaintext);
        size_t rem = (key_len - (p_len % key_len))% key_len;
	for (size_t i = p_len; i < p_len + rem; i++) {
		plaintext[i] = '_';
	}
	p_len += rem;
    	char *ciphertext = (char *)malloc(sizeof(char) * p_len);
	for (size_t i = 0; i < p_len; i++) {
		ciphertext[i] = plaintext[key_len*(i/key_len) + key[i % key_len]];
	}
	printf("Шифротекст: %s\n", ciphertext);
	printf("Шифротекст будет сохранен в файл cipher\n");
	if (write_to_file("cipher", ciphertext) != 0) {
		return -1;
	}
	memset(plaintext, 0, MAX_LEN);
	memset(key, 0, MAX_LEN);
	free(key);
	free(ciphertext);
	free(plaintext);
	return 0;
}


size_t decrypt_routine() {
        clearscr();
	char *ciphertext = (char *)malloc(sizeof(char) * MAX_LEN);
	size_t *key = (size_t *)malloc(sizeof(size_t) * MAX_LEN);
	memset(ciphertext, 0, MAX_LEN);
	memset(key, 0, MAX_LEN);
	size_t key_len = read_key(key);
	if (key_len == 0) {
		return -1;
	}
	printf("Полученный ключ:\n{ ");
	for (size_t i = 0; i < key_len; ++i) {
		printf("%zd ", key[i]);
	}
	printf("}\n");
	printf("Шифротекст будет прочитан из файла cipher!\n");
	if (read_from_file("cipher", ciphertext) != 0) {
		return -1;
	}
	printf("%s\n", ciphertext);
	size_t p_len = strlen(ciphertext);
	char *plaintext = (char *)malloc(sizeof(char) * p_len);
	for (size_t i = 0; i < p_len; i++) {
		plaintext[key[i % key_len] + key_len * (i/key_len)] = ciphertext[i];
	}
	printf("Открытый текст: %s\n", plaintext);
	printf("Открытый текст будет записан в файл plaintext!\n");
	if (write_to_file("plaintext", plaintext) != 0) {
		return -1;
	}
	free(plaintext);
	free(ciphertext);
	free(key);
	return 0;
}
	

int main(int argc, char *argv[], char *envp[]) {
	setlocale(LC_ALL, "Russian");
	printf("Данная програма реализует шифр перестановки для ключа произвольной длины\n");
	printf("Максимальная длина обрабатываемого сообщения и ключа - %d символов\n", MAX_LEN);
	char cmd = 'a';
	char stub;
	size_t status = 0;
	while (1) {
	       clearscr();	
	       printf("Для того, чтобы зашифровать сообщение введите E\nДля того, чтобы расшифровать сообщение введите D\nДля выхода введите Q\n");
	       scanf("%1s", &cmd); 
	       if (cmd == 'E' || cmd == 'e') {
	           status = encrypt_routine();
    	       } else if ((cmd == 'D') || (cmd == 'd')) {
	           status = decrypt_routine();
	       } else if ((cmd == 'Q') || (cmd == 'q')) {
		       break;
	       } else {
		   status = 0;    
		   printf("Ввод непонятен.\n");
	       }  
	       if (status == 0) {
			printf("Операция прошла успешно!\n");
		   	printf("Для продолжения нажмите любую клавишу...\n");
		   	scanf("%1s", &stub);
	       } else {
			printf("Ошибка исолнения операции\n");
	       }
	}
	return 0;
}
