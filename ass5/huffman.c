#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "node.h"
#include "heap.h"

// 허프만 트리를 순회하며 허프만 코드를 생성하여 codes에 저장
// leaf 노드에서만 코드를 생성
// strdup 함수 사용
// make_huffman_code 함수에서 호출
static void traverse_tree( tNode *root, char *code, int depth, char *codes[]);

// 새로운 노드를 생성
// 좌/우 subtree가 NULL(0)이고 문자(data)와 빈도값(freq)이 저장됨
// make_huffman_tree 함수에서 호출
// return value : 노드의 포인터
static tNode *newNode(unsigned char data, int freq);
////////////////////////////////////////////////////////////////////////////////
// 허프만 코드를 화면에 출력
void print_huffman_code(char *codes[])
{
	int i;
	
	for (i = 0; i< 256; i++)
	{
		printf("%d\t%s\n", i, codes[i]);
	}
}
////////////////////////////////////////////////////////////////////////////////
// 허프만 트리를 순회하며 허프만 코드를 생성하여 codes에 저장
static void traverse_tree( tNode *root, char *code, int depth, char *codes[]){
	
	if(root->left == NULL && root->right == NULL){
        code[depth]='\0';
        codes[root->data] = strdup(code);
		return;
    }
    if(root->left){
        code[depth] = '0';
        traverse_tree(root->left, code, depth + 1, codes);
    }
    if(root->right){
        code[depth] = '1';
        traverse_tree(root->right, code, depth + 1, codes);
    }
}
////////////////////////////////////////////////////////////////////////////////
// 새로운 노드를 생성
static tNode *newNode(unsigned char data, int freq){
	tNode *node = (tNode *)malloc(sizeof(tNode));
    node->data = data;
    node->freq = freq;
    node->left = NULL;
	node->right = NULL;
   
   return node;
}
////////////////////////////////////////////////////////////////////////////////
// 허프만 트리로부터 허프만 코드를 생성
// traverse_tree 함수 호출
void make_huffman_code( tNode *root, char *codes[])
{
	char code[256];
	
	traverse_tree( root, code, 0, codes);
}
////////////////////////////////////////////////////////////////////////////////
// 파일에 속한 각 문자(바이트)의 빈도 저장
int read_chars( FILE *fp, int ch_freq[]) {
	int character = 0;
    int byte = 0;

    while ((character = fgetc(fp)) != EOF) {
        ch_freq[character]++;
        byte++;
		if (feof(fp) != 0) break; 
    }
    return byte;
}

// 파일로부터 문자별 빈도(256개)를 읽어서 ch_freq에 저장
void get_char_freq( FILE *fp, int ch_freq[]){
	fread(ch_freq, sizeof(int), 256, fp);
}

// 허프만 코드에 대한 메모리 해제
void free_huffman_code( char *codes[]){
    for(int i = 0; i < 256; i++){
		free(codes[i]);
    }
}

// 허프만 트리를 생성
tNode *make_huffman_tree( int ch_freq[]){
	
    HEAP* heap = heapCreate(256);

    for (int i = 0; i < 256; i++) {
		tNode *node = newNode(i, ch_freq[i]);
        heapInsert(heap, node);
    }

    while (heap->last > 0) { 
        tNode* left = heapDelete(heap);
        tNode* right = heapDelete(heap);

        tNode* parent = newNode(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        heapInsert(heap, parent);
    }

    tNode* root = heapDelete(heap);
    heapDestroy(heap);

    return root;
}



// 허프만 트리 메모리 해제
void destroyTree( tNode *root){
	if (root == NULL) {
		return;
	}
	destroyTree(root->left);
	destroyTree(root->right);
	free(root);
}

// 입력 텍스트 파일(infp)을 허프만 코드를 이용하여 출력 파일(outfp)로 인코딩
int encoding( char *codes[], int ch_freq[], FILE *infp, FILE *outfp){
	char temp[200] = {0, };
    char byte = 0;
    long size = 0;
    int character = 0;
    int count = 0;
    unsigned long len = 0;
    
	while((character = fgetc(infp)) != EOF){
        if(feof(infp)) break;
        
		strcpy(temp, codes[character]);
        len = strlen(temp);
        
		for(int i = 0; i < len; ++i){
            byte = byte << 1;
            byte |= (temp[i] - '0');
            count++;
            if(count == 8){
                fwrite(&byte, sizeof(char), 1, outfp);
                count = 0;
                byte = 0;
            }
        }
    }
    fseek(outfp, 0, SEEK_END);
    size = ftell(outfp);
    return (int)size;
}


// 입력 파일(infp)을 허프만 트리를 이용하여 텍스트 파일(outfp)로 디코딩
void decoding( tNode *root, FILE *infp, FILE *outfp){
    tNode *temp = root;
    
	long size = 0;
    int bit = 0;
    char bitBuffer = 0;
    int current = 0;
	
    fseek(infp, 0, SEEK_END);
    size = ftell(infp);
    fseek(infp, 0, SEEK_SET);
    
	while(bit < size * 8){
        fread(&bitBuffer, sizeof(char), 1, infp);
        
		for(int i = 0; i < 8; ++i){
            current = bitBuffer & (1 << (7 - i));
            ++bit;
            
			if(!current) temp = temp->left;
            else temp = temp->right;
            
			if(temp->left == NULL && temp->right == NULL){
                fprintf(outfp, "%c", temp->data);
                temp = root;
            }
            
			if(bit == size * 8) break;
        }
    }
}