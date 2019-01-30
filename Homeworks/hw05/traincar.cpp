// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//
// =======================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "traincar.h"


// =======================================================================
// =======================================================================
//
//  HERE'S WHERE YOU SHOULD IMPLEMENT THE FUNCTIONS YOU PROTOTYPED IN
//  THE "traincar_prototypes.h" FILE.
//

//Implementation of PushBack
void PushBack(TrainCar* &train, TrainCar* newCar)
{
	TrainCar* p = train;

	//If train is empty, set it equal to the car we are pushing back
	if (train == NULL)
	{
		train = newCar;
		return;
	}

	//If not...get pointer p to the end of the train
	while (p->next != NULL)
	{
		p = p->next;
	}

	//Set p's next pointer to the new car, and the new car's prev pointer
	// to p, then the new car's next pointer to NULL
	p->next = newCar;
	newCar->prev = p;
	newCar->next = NULL;
}

//Function deletes all TrainCar nodes, preventing memory leaks
void DeleteAllCars(TrainCar* &train)
{
	TrainCar* p = train;

	//sets node we're on to p, sets train to next, then deletes p
    while (1)
    {
    	if (train == NULL) break;
        p = train;
        train = train->next;
        delete p;
    }
}

//Function takes multiple variables relating to statistics of the train car
//by reference, and edits them by going through the doubly linked list 
void TotalWeightAndCountCars(TrainCar* train, int& total_weight, 
	int& num_engines, int& num_freight_cars, int& num_passenger_cars, 
	int& num_dining_cars, int& num_sleeping_cars)
{
	TrainCar* p = train;
	total_weight = 0; num_engines = 0; num_freight_cars = 0; 
	num_dining_cars = 0; num_passenger_cars = 0; num_sleeping_cars = 0;

	//Goes through train and increments counters if we see the type of car
	//the counter correlates to
	while (1)
	{
		if (p->isEngine()) num_engines++; 
		if (p->isFreightCar()) num_freight_cars++;
		if (p->isPassengerCar()) num_passenger_cars++;
		if (p->isDiningCar()) num_dining_cars++;
		if (p->isSleepingCar()) num_sleeping_cars++;
		total_weight += p->getWeight();
		
		//break condition for the while(1) loop
		if (p->next == NULL)
		{
			break;
		}

		p = p->next;
	}
}

//Function uses formula to calculate the speed of a train
float CalculateSpeed(TrainCar* train)
{
	int total_horsepower = 0;
	int total_weight = 0;
	TrainCar* p = train;

	//loops through and finds the total horsepower and weight
	while(1)
	{
		if (p->isEngine()) total_horsepower += 3000; 
		total_weight += p->getWeight();

		//break condition
		if (p->next == NULL)
		{
			break;
		}

		p = p->next;
	}

	//returns a float of the speed of the train
	return ((total_horsepower*550.*3600)/(20*2.*5280*total_weight));
}

//Function calculates the average distance of all passenger cars to the 
//dining car, and if an engine or freight car is in the way, returns a 
//distance of -1, which is outputted as "inf" 
float AverageDistanceToDiningCar(TrainCar* train)
{			
	int c = 0;
	int cars = 0;
	TrainCar* p = train;
	bool dining = false;

	while(1)
	{
		//found a dining car...sets a boolean value to true
		if (p->isDiningCar()) {dining = true;}
		
		//if dining is still false and we come across a passenger car...
		if (p->isPassengerCar() && !dining)
		{
			//increment number of cars
			cars++;
			TrainCar* q = p;

			//loop as long as q isn't a dining car and isn't null
			while(!(q->isDiningCar()) && q->next != NULL)
			{
				//increment counter c, using the name of the language
				//this code is written in
				c++;

				//if it is an engine or freight car we come across, return
				//-1
				if (q->isEngine() || q->isFreightCar())
				{
					return -1;
				}

				q = q->next;
			}
		} 
		//however, if we did pass a dining car
		else if (p->isPassengerCar() && dining)
		{
			//increment amount of cars still
			cars++;
			TrainCar* q = p;

			//again, as long as q is not a dining car
			while(!(q->isDiningCar()))
			{
				c++;

				if (q->isEngine() || q->isFreightCar())
				{
					return -1;
				}

				//now, however, q is set to q->prev
				q = q->prev;
			}
		}


		if (p->next == NULL)
		{
			break;
		}

		p = p->next;
	}

	return (c*1.)/cars;
}

//Function calculates the closest engine to a sleeper car
int ClosestEngineToSleeperCar(TrainCar* train)
{
	TrainCar* p = train;
	int c = 0;
	int d = 0;

	while(1)
	{
		//If we come across a sleeping car
		if (p->isSleepingCar())
		{	
			TrainCar* q = p;
			//go through the train in both directions, and increment counters
			//as long as we don't find the engine
			while(!q->isEngine())
			{
				c++;
				if (q->next == NULL)
				{
					break;
				}
				q = q->next;
			}

			q = p;
			while(!q->isEngine())
			{
				d++;
				if (q->prev == NULL)
				{
					break;
				}
				q = q->prev;
			}
		}

		if (p->next == NULL)
		{
			break;
		}

		p = p->next;
	}

	//the maximum of the two counters returns the closest engine to the sleeper
	return max(c,d);
}

//Helper function, detatches a car in the front of a train
TrainCar* DetachCar(TrainCar*& train)
{
	//If it's the only car in the train, return that car
	if (train->next == NULL)
	{
		TrainCar* p = train;
		train = NULL;
		return p;
	}
	//If not, set the train to now point to the next one, and detach
	//the first one's next pointer, and return that car
	else
	{
		TrainCar* p = train;
		TrainCar* p_next = train->next;
		p_next->prev = NULL;
		p->next = NULL;
		train = p_next;
		return p;
	}
}

//Function implements the shipfreight algorithm
vector<TrainCar*> ShipFreight(TrainCar*& all_engines, TrainCar*& all_freight,
	int min_speed, int max_cars_per_train)
{
	//Creates the vector of TrainCar objects
	vector<TrainCar*> trains;

	//while both all_freight and all_engines are not null...
	while (all_freight != NULL && all_engines != NULL)
	{
		//make the first car of a train be an engine
		TrainCar* train = NULL;
		int i = 0;
		train = DetachCar(all_engines);
		
		//now loop again, with the same conditions
		while (all_freight != NULL && all_engines != NULL)
		{
			if (i == max_cars_per_train-1){break;}

			//if the train's speed is less than the min speed, we need
			//to add another engine
			if (CalculateSpeed(train) < (float)min_speed)
			{
				TrainCar* detached = DetachCar(all_engines);
				PushBack(train, detached);
			}
			//otherwise, we need to add another freight car
			else
			{
				TrainCar* detached = DetachCar(all_freight);
				PushBack(train, detached);
			}
			
			i++;
		}

		//outside of the nested loop, we make the check again, and if it is 
		//less, push back a car from the train back to all freight
		if (CalculateSpeed(train) < (float)min_speed)
		{
			TrainCar* temp = train;

			while (temp->next != NULL)
			{
				temp = temp->next;
			}

			TrainCar* temp_prev = temp->prev;
			temp_prev->next = NULL;
			temp->prev = NULL;
			PushBack(all_freight,temp);
		}

		//Push back the train to the vector of TrainCar* objects
		trains.push_back(train);
	}

	return trains;
}

//Helper function, removes the car if it's in the middle of the train
TrainCar* Remove(TrainCar*& train, TrainCar*& car)
{
	TrainCar* temp = car;

	//making sure it's not the first car in the train car object
	if(temp->prev != NULL)
	{
		TrainCar* temp_prev = car->prev;
	    TrainCar* temp_next = car->next;
	   	temp->next = NULL;
	    temp->prev = NULL;
	    temp_prev->next = temp_next;
	    temp_next->prev = temp_prev;
	    temp = temp_next;
	}
	//if it is, detach the car using the detach function
	else
	{
		car = DetachCar(train);
	}
    
    return car;
}

//Function implements the separate algorithm
void Separate(TrainCar*& train1, TrainCar*& train2, TrainCar*& train3)
{
	train2 = NULL;
	train3 = NULL;
	TrainCar* p = train1;
	int cars = 0, engines = 0, num_eng = 0, c;
	bool even, engine = false;

	//if we run across an engine, we increment an engine counter, and in 
	//both cases, we increment the total cars counter
	while (1)
	{
		if (p == NULL) break;

		if (p->isEngine())
		{
			engines++; 
			cars++;
		}
		else cars++;
		
		p = p->next;
	}

	//if the amount of cars is even, set even to true, else false
	if (cars%2 == 0) even = true;
	else even = false;

	if (even) c = (cars-engines)/2;
	else c = ((cars-engines)/2) + 1;

	//go through and add cars to train2, stopping at the c value determined
	//above
	for (int i = 0; i < c; i++)
	{
		TrainCar* car = DetachCar(train1);
		PushBack(train2,car);
	}

	TrainCar* p2 = train2;

	//if there's an engine in the remaining part of train1, set engine
	//to true
	while(1)
	{
		if (p2 == NULL) break;

		if (p2->isEngine())
		{
			engine = true;
			num_eng++;
		}  

		p2 = p2->next;
	}

	//if engine is false
	if (!engine)
	{
		p = train1;
		while(1)
		{
			if (p == NULL) break;

			//takes an engine from train1 and push it back to train2
			if (p->isEngine())
			{
				TrainCar* detached_engine = Remove(train1, p);
				PushBack(train2, detached_engine);
				break;
			}
			p = p->next;
		}
	}
	//if engines in remaining part of train1 equals total engines in the train,,
	//assures code works for multiple engines well
	else if (num_eng == engines)
	{
		p = train2;
		
		while (p->next != NULL)
		{
			if (p->isEngine()) break;
		}

		//remove an engine from train2, push it back to train3, detach one 
		//from train1, and push it back to train2
		TrainCar* engine = Remove(train2, p);
		PushBack(train3, engine);
		TrainCar* extra_car = DetachCar(train1);
		PushBack(train2, extra_car);
	}
	//if engines is greater than total engines minus the amount of engines...
	else if (num_eng > (engines-num_eng) && num_eng % 2 == 1)
	{
		while (train1->next != NULL)
		{
			if (train1->isEngine()) break;

			//detach a car from train1 and push it back to train2
			TrainCar* extra_car = DetachCar(train1);
			PushBack(train2, extra_car);
		}
	}

	//add cars to train3
	while(train1 != NULL)
	{
		TrainCar* car = DetachCar(train1);
		PushBack(train3, car);
	}

}