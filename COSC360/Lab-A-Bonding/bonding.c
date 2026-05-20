/*
Connor Riley
4/28/2024
Bonding Lab Plank cs360
H2O bonding simulation using threads for each atom
*/
#include "bonding.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*node struct for each atom/thread */
struct node {
  int id;
  int hyd1;
  int hyd2;
  int oxy;
  pthread_cond_t *thread_conditional;
  struct node *flink;
  struct node *blink;
};
/*global values*/
struct global {
  struct node *hyd;
  struct node *oxy;
  pthread_mutex_t *lock;
  char *verbosity;
};
/*initialize function to set global to defualt values*/
void *initialize_v(char *verbosity) {
  struct global *glob;
  glob = (struct global *)malloc(sizeof(struct global));
  glob->hyd = NULL;
  glob->oxy = NULL;
  glob->lock = new_mutex();
  glob->verbosity = verbosity;
  return glob;
}
/*hydrogen thread function*/
void *hydrogen(void *arg) {
  /*initialize values to default*/
  struct bonding_arg *ba;
  struct global *glob;
  struct node *node;
  struct node n;

  ba = (struct bonding_arg *)arg;
  glob = (struct global *)ba->v;
  n.id = ba->id;
  n.hyd1 = -1;
  n.hyd2 = -1;
  n.oxy = -1;
  n.thread_conditional = new_cond();
  n.flink = NULL;
  n.blink = NULL;
  /*call lock on global to prevent other threads from accessing data structure while
  the hydrogen function is */
  pthread_mutex_lock(glob->lock);
  if (glob->oxy != NULL && glob->hyd != NULL) {
    // if (strchr(glob->verbosity, 'H') != NULL) {
    //   printf("hydrogen thread %d bond\n", n.id);
    // }
    /*set all your hydrogens and oxygens to the correct values*/
    n.hyd1 = glob->hyd->id;
    glob->hyd->hyd1 = glob->hyd->id;
    glob->oxy->hyd1 = glob->hyd->id;

    n.hyd2 = n.id;
    glob->hyd->hyd2 = n.id;
    glob->oxy->hyd2 = n.id;

    n.oxy = glob->oxy->id;
    glob->hyd->oxy = glob->oxy->id;
    glob->oxy->oxy = glob->oxy->id;
    // unblock the other hyd and oxy threads
    pthread_cond_signal(glob->hyd->thread_conditional);
    pthread_cond_signal(glob->oxy->thread_conditional);
    // remove global->hyd from hydrogen list
    glob->hyd = glob->hyd->flink;
    if (glob->hyd != NULL) {
      glob->hyd->blink = NULL;
    }
    // remove global->oxy from oxygen list
    glob->oxy = glob->oxy->flink;
    if (glob->oxy != NULL) {
      glob->oxy->blink = NULL;
    }
    // unlock mutex, free conditional and call the Bond function
    pthread_mutex_unlock(glob->lock);
    free(n.thread_conditional);
    return (void *)Bond(n.hyd1, n.hyd2, n.oxy);
  }
  // if we didnt have an oxy or hyd then we need to set them
  if (glob->hyd == NULL) {
    glob->hyd = &n;
  } else {
    node = glob->hyd;
    while (node->flink != NULL) {
      node = node->flink;
    }
    node->flink = &n;
    n.blink = node;
  }

  // if (strchr(glob->verbosity, 'H') != NULL) {
  //   printf("hyd thread %d waiting\n", n.id);
  // }
  // call wait
  pthread_cond_wait(n.thread_conditional, glob->lock);

  // if (strchr(glob->verbosity, 'H') != NULL) {
  //   printf("hyd thread %d bonding\n", n.id);
  // }
  // unlock mutex, free conditional and call the Bond function
  pthread_mutex_unlock(glob->lock);
  free(n.thread_conditional);
  return (void *)Bond(n.hyd1, n.hyd2, n.oxy);
}
/*oxygen function is quite similar to the hydrogen function but we are just dealing
with 2 hydrogen threads instead so we will be using the flink of hyd to get the second*/
void *oxygen(void *arg) {
  struct bonding_arg *ba;
  struct global *glob;
  struct node *node;
  struct node n;

  ba = (struct bonding_arg *)arg;
  glob = (struct global *)ba->v;
  n.id = ba->id;
  n.hyd1 = -1;
  n.hyd2 = -1;
  n.oxy = -1;
  n.thread_conditional = new_cond();
  n.flink = NULL;
  n.blink = NULL;

  pthread_mutex_lock(glob->lock);
  if (glob->hyd != NULL && glob->hyd->flink != NULL) {
    // if (strchr(glob->verbosity, 'O') != NULL) {
    //   printf("oxygen thread %d found bond\n", n.id);
    // }
    //set the hydrogen to the global hyd 
    n.hyd1 = glob->hyd->id;
    glob->hyd->hyd1 = glob->hyd->id;
    glob->hyd->flink->hyd1 = glob->hyd->id;
    //set the second hyd to the flink of h1
    n.hyd2 = glob->hyd->flink->id;
    glob->hyd->hyd2 = glob->hyd->flink->id;
    glob->hyd->flink->hyd2 = glob->hyd->flink->id;
    //set the oxygen to self
    n.oxy = n.id;
    glob->hyd->oxy = n.id;
    glob->hyd->flink->oxy = n.id;

    pthread_cond_signal(glob->hyd->thread_conditional);
    pthread_cond_signal(glob->hyd->flink->thread_conditional);

    glob->hyd = glob->hyd->flink;
    glob->hyd->blink = NULL;
    glob->hyd = glob->hyd->flink;
    if (glob->hyd != NULL) {
      glob->hyd->blink = NULL;
    }

    pthread_mutex_unlock(glob->lock);
    free(n.thread_conditional);
    return (void *)Bond(n.hyd1, n.hyd2, n.oxy);
  }
  
  if (glob->oxy == NULL) {
    glob->oxy = &n;
  } else {
    node = glob->oxy;
    while (node->flink != NULL) {
      node = node->flink;
    }
    node->flink = &n;
    n.blink = node;
  }

  // if (strrchr(glob->verbosity, 'O') != NULL) {
  //   printf("oxygen %d waiting\n", n.id);
  // }
  pthread_cond_wait(n.thread_conditional, glob->lock);
  // if (strchr(glob->verbosity, 'O') != NULL) {
  //   printf("oxygen %d bonding\n", n.id);
  // }

  pthread_mutex_unlock(glob->lock);
  free(n.thread_conditional);
  return (void *)Bond(n.hyd1, n.hyd2, n.oxy);
}
