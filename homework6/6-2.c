#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct list_node_s {
    int data;
    struct list_node_s* next;
};

int thread_count;
struct list_node_s* head_p = NULL;
pthread_rwlock_t rwlock;
// pthread_mutex_t mutex;

int* tasks;
int* rand_numbers;

int data_count;
int num_member = 0;
int num_insert = 1;
int num_delete = 2;

int count_member = 0;
int count_insert = 0;
int count_delete = 0;
int count_linked = 0;
int already_in_list = 0;
int not_in_list = 0;
int valid_delete = 0;

void Read_userinput(int* a, int* b);
void create_rand_nums(int task_n_num_size, int thread_size);
void create_rand_list(int num_size);
void display(struct list_node_s*);

void *task_queue(void* rank);

int Member(int value, struct list_node_s* head_p);
int Insert(int value, struct list_node_s** head_p);
int Delete(int value, struct list_node_s** head_p);

int main()
{
    int a, b;
    long    thread;
    pthread_t* thread_handles;

    Read_userinput(&a, &b);
    create_rand_nums(a, b);
    create_rand_list(20);

    thread_handles = malloc(thread_count*sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL,
            task_queue, (void*) thread);

    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }	  

    display(head_p);

    printf("Task Count: %d\n", a);
    printf("Thread Count: %d\n", thread_count);
    printf("Items in Linked List before running program: 20\n");
    printf("---------------------------\n");
    printf("Items inside Linked List: %d\n", count_linked);
    printf("Member Calls: %d\n", count_member);
    printf("Insert Calls: %d\n", count_insert);
    printf("Delete Calls: %d\n", count_delete);
    printf("---------------------------\n");
    printf("Already in list when insert call: %d\n", already_in_list);
    valid_delete = count_delete - not_in_list;
    printf("Valid operation when delete call: %d\n", valid_delete);

    printf("\n");
    
    free(tasks);
    free(rand_numbers);
    return 0;
}

void Read_userinput(int* a, int* b)
{
    printf("Please enter value for task count: ");
    scanf("%d", a);

    printf("Please enter value for thread count: ");
    scanf("%d", b);
}

void create_rand_nums(int task_n_num_size, int thread_size)
{
    thread_count = thread_size;
    data_count = task_n_num_size;

    tasks = malloc(task_n_num_size*sizeof(int));
    rand_numbers = malloc(task_n_num_size*sizeof(int));

    int i;
    for (i = 0; i < task_n_num_size; i++)
    {
        tasks[i] = rand() % 3;
        // printf("task number: %d\n", tasks[i]);

        rand_numbers[i] = rand() % 99;
        // printf("random number: %d\n", rand_numbers[i]);
    }
}

void create_rand_list(int num_size)
{
    int i;
    for (i = 0; i < num_size; i++)
    {
        Insert(rand() % 99, &head_p);
    }
}

void display(struct list_node_s *head)
{
    printf("Values inside Linked List\n");
    while (head != NULL)
    {
        count_linked++;
        printf("%d ", head->data);
        head = head->next;
    }

    printf("\n---------------------------\n");
}
    
void *task_queue(void* rank)
{
    int my_rank = (long) rank;

    int local_data_count = data_count / thread_count;
    int first_thread = my_rank * local_data_count;
    int last_thread = (my_rank + 1) * local_data_count - 1;
    // double* local_data = malloc(local_data_count*sizeof(double));
    int i;

    // pthread_mutex_lock(&mutex);
    printf("my rank: %d\n", my_rank);

    for (i = first_thread; i <= last_thread; i++)
    {   
        printf("***************************\n");

        if (tasks[i] == num_member)
        {
            pthread_rwlock_rdlock(&rwlock);
            printf("Member Task\n");
            Member(rand_numbers[i], head_p);
            count_member++;
            pthread_rwlock_unlock(&rwlock);
        }
        else if (tasks[i] == num_insert)
        {
            pthread_rwlock_rdlock(&rwlock);
            printf("Insert Task\n");
            Insert(rand_numbers[i], &head_p);
            count_insert++;
            pthread_rwlock_unlock(&rwlock);
        }
        else if (tasks[i] == num_delete)
        {
            pthread_rwlock_rdlock(&rwlock);
            printf("Delete Task\n");
            Delete(rand_numbers[i], &head_p);
            count_delete++;
            pthread_rwlock_unlock(&rwlock);
        }
        printf("random number: %d\n", rand_numbers[i]);
    }
    printf("***************************\n");

    // pthread_mutex_unlock(&mutex);
    return NULL;
}

int Insert(int value, struct list_node_s** head_pp) {
   struct list_node_s* curr_p = *head_pp;
   struct list_node_s* pred_p = NULL;
   struct list_node_s* temp_p;
   
   while (curr_p != NULL && curr_p->data < value) {
      pred_p = curr_p;
      curr_p = curr_p->next;
   }

   if (curr_p == NULL || curr_p->data > value) {
      temp_p = malloc(sizeof(struct list_node_s));
      temp_p->data = value;
      temp_p->next = curr_p;
      if (pred_p == NULL)
         *head_pp = temp_p;
      else
         pred_p->next = temp_p;
      return 1;
   } else { /* value in list */
      printf("%d is already in the list\n", value);
      already_in_list++;
      return 0;
   }
}  /* Insert */

int Delete(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    /* Find value */
    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) { /* first element in list */
            *head_pp = curr_p->next;
    #        ifdef DEBUG
            printf("Freeing %d\n", value);
    #        endif
            free(curr_p);
        } else { 
            pred_p->next = curr_p->next;
    #        ifdef DEBUG
            printf("Freeing %d\n", value);
    #        endif
            free(curr_p);
        }
        return 1;
    } else {
        printf("%d is not in the list\n", value);
        not_in_list++;
        return 0;
    }
}  /* Delete */

int  Member(int value, struct list_node_s* head_p) {
   struct list_node_s* curr_p;

   curr_p = head_p;
   while (curr_p != NULL && curr_p->data < value)
      curr_p = curr_p->next;

   if (curr_p == NULL || curr_p->data > value) {
      printf("%d is not in the list\n", value);
      return 0;
   } else {
      printf("%d is in the list\n", value);
      return 1;
   }
}  /* Member */

// int Member(int value) 
// {
//     struct list_node_s* temp_p;

//     pthread_mutex_lock(&head_p_mutex);
//     temp_p = head;
//     while (temp_p != NULL && temp_p -> data < value) 
//     {
//         if (temp_p -> next != NULL)
//             pthread_mutex_lock(&(temp_p -> next -> mutex));

//         if (temp_p == head)
//             pthread_mutex_unlock(&head_p_mutex);

//         pthread_mutex_unlock(&(temp_p -> mutex));
//         temp_p = temp_p -> next;
//     }
//     if (temp_p == NULL || temp_p -> data > value) 
//     {
//         if (temp_p == head)
//             pthread_mutex_unlock(&head_p_mutex);
//         if (temp_p != NULL)
//             pthread_mutex_unlock(&(temp_p -> mutex));
//         return 0;
//     } 
//     else 
//     {
//         if (temp_p == head)
//             pthread_mutex_unlock(&head_p_mutex);

//         pthread_mutex_unlock(&(temp_p -> mutex));
//         return 1;
//     }
// }

// void *task_queue(void* rank) {
//    long my_rank = (long) my_rank;
   
//    pthread_barrier_wait(&barrier_p);
//    if(my_rank == 0){
//    	Insert(8,&head_p);
//    }else{
// 	Member(8,head_p);
//    }

//    return NULL;
// }  /* Thread_work */