# Compile & Run:
```
$ gcc q2.c
$ ./a.out
```

`Note`: I am printing Timestamps of events also , to remove time stamps , uncomment Line No `10` in `q2.c` , and then recompile.

`Note`: The File q2.c is not readeable , because of printing timestamps code, there is a much neater code in `q2CleanCode.txt`

# Algorithm:
## Chef:
    ```
    sleep till entry time.

    while His Time Not Completed:
        Wait For Some Order.

        If doable within His time, Pick It Up From the List Of Orders:
            Check If Ingrediants Are Available.

            If Not Available:
                Notify The Customer.
            Else:
                Collect Ingrediants (3 Sec)
                Wait For an Oven To be allocated.
                Bake in Oven For t-3 seconds.
                Send Pizza To PickUp Area.
        
        Else:
            Pass On list to next Chef, Without disturbing List.
            Recheck After 1 Second.
    
    Update the Number of Completed Chefs , and Exit From Restaurant.
    ```

## Customer:
    ```
    sleep Until Entry time.

    Check there are any Chefs , who is working, or yet to come to work.

    if No:
        return with disappointment.
    
    Otherwise:
    
    Check if ingrediants are available For Atleast One Of His Pizza.
    If YES:
        Place Order.
    Otherwise:
        Leave the restaurant.
        -X-X-X-
    
    Write His Order in the List.
    Start going to PickUp area (meanwhile also keeping an eye if the order gets cancelled).
    If order gets cancelled in between, Leave the restaurant.
    Otherwise:

    wait for his pizzas to arrive, and check for his pizzas every 1 second.

    after picking all Available pizzas , leave the restaurant.
    ```

# Implementation:
## Ingrediants:
* Stored in a global array `Ing`
* Mutex Lock `IngStore` Is Used to acheive mutex at the ingrediant Pickup Area.

## Customers:
* One Thread Per Customer.
* Storing all Customers as an array `CustArr`
* Each Customer Has the following Structure:
    ```
    int Entry;
    int NoOrd;
    int *Order;
    pthread_mutex_t OrderMtx;
    int NoOfOrdCompl;
    int NoOfOrdSucc;
    sem_t Reject;
    pthread_t thd;
    ```
    
    1. `Entry` - Entry Time
    1. `NoOrd` - No Of Orders.
    1. `Order` - List Of Pizzas Ordered.
    1. `OrderMtx` - Mutex to Update `NoOfOrdCompl`, `NoOfOrdSucc`
    1. `NoOfOrdCompl` - No Of Orders completed ( Either pizzas completed / Pizzas rejected).
    1. `NoOfOrdSucc` - No Of Orders Successfull ( Pizzas completed Baking ).
    1. `Reject` - TO Handle Rejectione wile going to PickUp Area.
    1. `thd` - Corresponding thread object.

* using `sem_timedwait()` to handle rejections while reaching PickUp Area.

## Chef:
* One Thread Per Chef.
* Storing all Chefs as an array `ChefArr`
* Structure:
    ```
    int Entry;
    int exit;
    pthread_t thd;
    ```

    1. `Entry` - Entry Time
    1. `NoOrd` - No Of Orders.
    1. `thd` - Corresponding thread object.

## Order List:
* List Of Orders awaiting to be processed.
* used a Linke dList to Store.
* Used Mutex Locks `ListLock` To ensure Mutex ( Fo customers and chefs).
* Used Semaphor `PizQueue` To notify Chef of incoming order.
* Stores:
    ```
    int QCust;
    int QPizza;
    ```

    1. `QCust`: Customer Number.
    1. `QPizza`: Pizza Number.

## Pick Up List:
* List Of Orders awaiting to be Pciked Up.
* used a Linked List to Store.
* Used Mutex Locks `PickUpMtx` To ensure Mutex ( Fo customers and chefs).
* Used Semaphor `PickUpQueue` To notify Customers of some arrived order.
* Stores:
    ```
    int QCust;
    int QPizza;
    ```

    1. `QCust`: Customer Number.
    1. `QPizza`: Pizza Number.


## Ovens:
* Semaphor `Oven` Is used to keep track of Ovens.
* only called by Chefs.
* baking simulated by making the chef sleep.

## Pizza:
* Stored as:
    ```
    int Num;
    int time;
    int NumIng;
    int *ingredients;
    ```

    * `Num`: Pizza Number.
    * `time`: Baking Time.
    * `NumIng`: Number of ingrediants needed.
    * `ingrediants`: List of Ingrediants needed


# Follow Up Questions:
## Q1:
The pick-up spot now has a stipulated amount of pizzas it can hold. If the 
pizzas are full, chefs route the pizzas to a secondary storage. How would you 
handle such a situation?

## Sol:
* As And When any Pizza kept in Pickup Area is given off to cutomer , i would replace the empty slot with some pizza in secondary storage.
* This can be implemented using a semaphor to keep track of empty slots in pickup area.

## Q2:
Each incomplete order affects the ratings of the restaurant. Given the past 
histories of orders, how would you re-design your simulation to have lesser 
incomplete orders? Note that the rating of the restaurant is not affected if the 
order is rejected instantaneously on arrival. 

## Sol:
keep 2 counting variables of ingrediants (one is the actual count, Other is one that is ust an estimate ).
as soon as customer comes, reject if the count of estimated ingrediant is not sufficient.) Otherwise accep the order and decrement estimated count.
this avoid the situation of first accepted and rejected after some time cases.

## Q3:
Ingredients can be replenished on calling the nearest supermarket. How 
would your drive-thru rejection / acceptance change based on this? 

## Sol:
I would accept all customer , But inform them to wait for a more time than usual. I will assign a worker to go to market, he keep checking the storage now and then , and then depending on availability , travel to market at regular intervals of time.



<!-- #ifdef DEBUG_MODE
printf("%d: ",(int) difftime(time(0),StartTime));
#endif
printf -->