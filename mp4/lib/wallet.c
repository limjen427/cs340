#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wallet.h"

/**
 * Initializes an empty wallet.
 */
void wallet_init(wallet_t *wallet) {
  // Implement
  pthread_mutex_init(&wallet->lock, NULL);
  pthread_cond_init(&wallet->cond, NULL);
  wallet->head = NULL;
}




/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  // Implement this
  wallet_resource *cur_res = wallet->head;

  while (cur_res != NULL) {
    pthread_mutex_lock(&wallet->lock);

    if (cur_res->resource_name != NULL && strcmp(cur_res->resource_name, resource) == 0) {
      pthread_mutex_unlock(&wallet->lock);
      return cur_res->amount;
    }

    cur_res = cur_res->next;
    pthread_mutex_unlock(&wallet->lock);
  }
  return 0;
}

/**
 * Find if 'resource' is in the wallet and if not initialize it
*/
void find_resource(wallet_t *wallet, const char *resource) {
  wallet_resource *cur_res = wallet->head;

  while (cur_res != NULL) {
    if (cur_res->resource_name != NULL && strcmp(cur_res->resource_name, resource) == 0) {
      return;
    }

    cur_res = cur_res->next;
  }

  wallet_resource *new_head = malloc(sizeof(wallet_resource));
  new_head->amount = 0;
  new_head->resource_name = resource;
  new_head->next = wallet->head;
  wallet->head = new_head;
  
  return;
}


/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource can be satisfied.
 *   (Ths function MUST WAIT until the wallet has enough resources to satisfy the request;
 *    there are several ways to accomplish this waiting and it does not have to be fancy.)
 */
void wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  // Implement this
  pthread_mutex_lock(&wallet->lock);
  find_resource(wallet, resource);
  pthread_mutex_unlock(&wallet->lock);

  wallet_resource *cur_res = wallet->head;

  while (cur_res != NULL) {
    pthread_mutex_lock(&wallet->lock);

    if (cur_res->resource_name != NULL && strcmp(cur_res->resource_name, resource) == 0) {

      while (cur_res->amount + delta < 0) {
        pthread_cond_wait(&wallet->cond, &wallet->lock);
      }

      cur_res->amount += delta;
      
      pthread_cond_broadcast(&wallet->cond);
      pthread_mutex_unlock(&wallet->lock);
      break;
    } 
    cur_res = cur_res->next;
    pthread_mutex_unlock(&wallet->lock);
  }

}


/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {
  // Implement this
  wallet_resource *cur_res = wallet->head;
  wallet_resource *temp;

  while (cur_res != NULL) {
    temp = cur_res->next;
    free(cur_res);
    cur_res = temp;
  }

  pthread_mutex_destroy(&wallet->lock);
  pthread_cond_destroy(&wallet->cond);
}