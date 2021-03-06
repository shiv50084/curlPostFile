#include <stdio.h>
#include <string.h>
 
#include <curl/curl.h>
 
int main(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;
 
  struct curl_httppost *post=NULL;
  struct curl_httppost *last=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
 
  curl_global_init(CURL_GLOBAL_ALL);
 
    curl_formadd(&post,
                 &last,
                 CURLFORM_COPYNAME, "device",
                 CURLFORM_COPYCONTENTS, "1423638030", CURLFORM_END);
 
    curl_formadd(&post, 
                 &last, 
                 CURLFORM_COPYNAME, "sku",
                 CURLFORM_COPYCONTENTS, "H6104", CURLFORM_END);
 
    curl_formadd(&post, 
                 &last, 
                 CURLFORM_COPYNAME, "file",
                 CURLFORM_FILE, "a.txt", CURLFORM_END);
 
 
  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */
  headerlist = curl_slist_append(headerlist, buf);
  if(curl) {
    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, "http://xxxxxxxxxxxx/v1/snapshotUrl");
    if ( (argc == 2) && (!strcmp(argv[1], "noexpectheader")) )
      /* only disable 100-continue header if explicitly requested */
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
 
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */
    curl_easy_cleanup(curl);
 
    /* then cleanup the formpost chain */
    curl_formfree(post);
    /* free slist */
    curl_slist_free_all (headerlist);
  }
  return 0;
}

