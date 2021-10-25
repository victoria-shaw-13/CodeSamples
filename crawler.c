/*
 *crawler function which loops over pages to explore until list is recursively exhaused
 * uses a Bag, hastable, and webpage data Structure that is also built. 
 *Victoria Shaw 2021
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"
#include "pagedir.h"

/**functions**/
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth );
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);  

/**global variables**/
char* seedURL;
char* pageDirectory;
int maxDepth;
/**main**/

int main(const int argc, char* argv[]){
  parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
  crawl(seedURL, pageDirectory, maxDepth);
}

//specify errors 
static void
parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{
  if(argc != 4) {
    fprintf(stderr,"incorrect number of arguements\n");
    exit(1);
  }
  if(argv[1] != NULL){
    *seedURL = normalizeURL(argv[1]);
    if(!isInternalURL(*seedURL))
    {
      //free(seedURL);
      fprintf(stderr, "url is not internalized\n");
      exit(1);
    }
  }else{
    fprintf(stderr, "seedURL is NULL\n");
    exit(2);
  }
  if(argv[2]!=NULL&& pagedir_init(argv[2]))
    {
     *pageDirectory = argv[2];
    }else{
      if(argv[2]==NULL){
        fprintf(stderr,"null directory\n");
      }
      fprintf(stderr, "could not intialize directory\n");
      exit(2);
    }
    if(argv[3]!= NULL){
      if(sscanf(argv[3], "%d", maxDepth)==EOF)
      {
        fprintf(stderr, "error in retrieving number\n");
        exit(2);
      }
      if(*maxDepth<0 || *maxDepth>10){
        fprintf(stderr, "out of range\n");
      exit(2);
      }
    }
    return;
     
}


static void
crawl(char* seedURL, char* pageDirectory, const int maxDepth){
  //i don't remember if you need to initalize vairables
  int docID = 0; 
  hashtable_t* pagesSeen  =  hashtable_new(200); 
  //inserting starting webpage 
  hashtable_insert(pagesSeen,seedURL, " ");
  webpage_t* webpage = webpage_new(seedURL, 0,NULL); 
  bag_t* bag = bag_new();
  bag_insert(bag, webpage);
  //while bag is empty 
  webpage_t* currPage;
  while((currPage=bag_extract(bag))!=NULL){
    if(webpage_fetch(currPage)){
      printf("%d fetched: %s\n",webpage_getDepth(currPage),webpage_getURL(currPage));
      docID++;
      pagedir_save(currPage, pageDirectory,docID);
      if(webpage_getDepth(currPage)<maxDepth){
        pageScan(currPage,bag,pagesSeen);
      }
    }
    //test if this is ok 
     webpage_delete(currPage);
    
  }
  hashtable_delete(pagesSeen,NULL);
  bag_delete(bag,NULL);

  return; 
}


static void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
  printf("%d scanning: %s\n",webpage_getDepth(page),webpage_getURL(page));
  webpage_fetch(page);
  int pos = 0;
  char* URL;
  while ((URL = webpage_getNextURL(page, &pos)) != NULL){
    printf("%d found: %s\n",webpage_getDepth(page),URL);
    char* mallocURL = normalizeURL(URL);
    if(isInternalURL(URL)){
      if(hashtable_insert(pagesSeen, mallocURL," ")){
        printf("%d added: %s\n",webpage_getDepth(page),mallocURL);
        webpage_t* new = webpage_new(mallocURL, webpage_getDepth(page) + 1, NULL);
        bag_insert(pagesToCrawl, new);
      }else{
        printf("%d IgnDupl: %s\n",webpage_getDepth(page),mallocURL);
        free(mallocURL);
      }
    }else{
      //is external 
      printf("%d IgnExtrn: %s\n",webpage_getDepth(page),URL);
      free(mallocURL);
    }
    free(URL);
  }
      
}

        
    
  