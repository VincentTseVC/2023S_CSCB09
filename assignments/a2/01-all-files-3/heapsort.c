#include "heapsort.h"
#include "customer.h"

#include <stdio.h>
#include <string.h>

int safe_compare_names(char *name1, char *name2) {
    int len1 = strnlen(name1, CUSTOMER_NAME_MAX);
    int len2 = strnlen(name2, CUSTOMER_NAME_MAX);
    return strncmp(name1, name2, len1 < len2 ? len1 : len2);
}

void swap_customers(FILE *fp, long i, long j) {
    customer temp1, temp2;

    fseek(fp, sizeof(customer) * i, SEEK_SET);
    fread(&temp1, sizeof(customer), 1, fp);

    fseek(fp, sizeof(customer) * j, SEEK_SET);
    fread(&temp2, sizeof(customer), 1, fp);

    fseek(fp, sizeof(customer) * i, SEEK_SET);
    fwrite(&temp2, sizeof(customer), 1, fp);

    fseek(fp, sizeof(customer) * j, SEEK_SET);
    fwrite(&temp1, sizeof(customer), 1, fp);
}

void heapify(FILE *fp, long i, long n) {
    long largest = i;
    long left = 2*i + 1;
    long right = 2*i + 2;

    //       curr
    customer root, leftChild, rightChild, largestNode;

    // move the file pointer to ith node in the file
    fseek(fp, sizeof(customer) * i, SEEK_SET);
    fread(&root, sizeof(customer), 1, fp);

    // has left child
    if (left < n) {
        // move the file pointer to the left child node in the file
        fseek(fp, sizeof(customer) * left, SEEK_SET);
        fread(&leftChild, sizeof(customer), 1, fp);

        if (leftChild.loyalty > root.loyalty) {
            largest = left;
        }
        else if (leftChild.loyalty == root.loyalty && safe_compare_names(leftChild.name, root.name) > 0) {
            largest = left;
        }
    }

    // has right child
    if (right < n) {
        // move the file pointer to the right child node in the file
        fseek(fp, sizeof(customer) * right, SEEK_SET);
        fread(&rightChild, sizeof(customer), 1, fp);

        // move the file pointer to the largest node (i or left) in the file
        fseek(fp, sizeof(customer) * largest, SEEK_SET);
        fread(&largestNode, sizeof(customer), 1, fp);

        if (rightChild.loyalty > largestNode.loyalty) {
            largest = right;
        }
        else if (rightChild.loyalty == largestNode.loyalty && safe_compare_names(rightChild.name, largestNode.name) > 0) {
            largest = right;
        }
    }

    if (largest != i) {
        swap_customers(fp, i, largest);
        heapify(fp, largest, n);
    }

}

int heapsort(const char *filename) {

    FILE *fp = fopen(filename, "r+b");

    if (fp == NULL) return 0;

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    long num_customers = filesize / sizeof(customer);
    fseek(fp, 0, SEEK_SET);

    // Heapify the entire file (array) [Convert it into a max heap]
    for (long i = num_customers / 2 - 1; i >= 0; i--) {
        // "heapify at v"
        heapify(fp, i, num_customers);
    }

    // In-place heapSort, keep extrac-max and put answer to the end
    for (long i = num_customers - 1; i >= 0; i--) {
        swap_customers(fp, 0, i);
        heapify(fp, 0, i); // n -> i
    }

    fclose(fp);
    return 1;
}

// submit 之前把main刪掉！
int main() {
    heapsort("sample.dat");
}