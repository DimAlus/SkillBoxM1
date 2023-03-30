#include <iostream>
#include <string>
#include <math.h>
#include <vector>

class Wheel {
public:
	float diametr;
	Wheel(float d) : diametr(d) {}
};

class Engine {
public:
	float power;

	Engine(float pwr) : power(pwr) {}
};

class Vehicle {
	friend std::ostream& operator<< (std::ostream& out, Vehicle& me);
protected:
	virtual std::ostream& print(std::ostream& out) = 0;
	virtual std::string GetInfo() = 0;
};

std::ostream& operator<< (std::ostream& out, Vehicle& me) {
	return me.print(out);
}

class WaterVehicle : virtual public Vehicle {
protected:
	float precipitation;

	virtual std::string GetInfo() override {
		std::string res = "";
		res += "WaterVehicle Precipitation: " + std::to_string(int(precipitation));
		return res;
	}


	virtual std::ostream& print(std::ostream& out) {
		out << GetInfo();
		return out;
	}
public:
	WaterVehicle(float prec) : precipitation(prec) {}
	~WaterVehicle() {}
};

class RoadVehicle : virtual public Vehicle {
protected:
	float groundClearance;
	Wheel* Wheels;
	int countWeels;

	RoadVehicle(float clrnc) : groundClearance(clrnc), Wheels(nullptr), countWeels(0) {}
	void Init(Wheel* wheels, int count) {
		Wheels = wheels; countWeels = count;
	}
	~RoadVehicle() {
		if (Wheels)
			delete[] Wheels;
	}

	virtual std::string GetInfo() override {
		std::string res = "";
		res += "Wheels:";
		for (int i = 0; i < countWeels; i++)
			res += " " + std::to_string(int(Wheels[i].diametr));
		res += " Ride height: " + std::to_string(int(groundClearance));
		return res;
	}


	virtual std::ostream& print(std::ostream& out) {
		out << GetInfo();
		return out;
	}
};

class Bicycle : virtual public RoadVehicle {
protected:

	virtual std::string GetInfo() override {
		return "Bicycle " + RoadVehicle::GetInfo();
	}

public:
	Bicycle(Wheel w1, Wheel w2, float grndClrnc) : RoadVehicle(grndClrnc) {
		Wheel* wls = new Wheel[2]{ w1,w2 };
		Init(wls, 2);
	}
};

class Car : virtual public RoadVehicle {
protected:
	Engine engine;

	virtual std::string GetInfo() override {
		return "Bicycle Engine: " + std::to_string(int(engine.power)) + " " + RoadVehicle::GetInfo();
	}


public:
	Car(Engine eng, Wheel w1, Wheel w2, Wheel w3, Wheel w4, float grndClrnc) : RoadVehicle(grndClrnc), engine(eng) {
		Wheel* wls = new Wheel[4]{ w1,w2,w3,w4 };
		Init(wls, 4);
	}

	float GetPower() { return engine.power; }
};


float getHighestPower(std::vector<Vehicle*> v) {
	float mx = 0;;
	Car* car;
	for (int i = 0; i < v.size(); i++)
		if (car = dynamic_cast<Car*>(v[i]))
			if (car->GetPower() > mx)
				mx = car->GetPower();
	return mx;
}
int main()
{
	Car c(Engine(150), Wheel(17), Wheel(17), Wheel(18), Wheel(18), 150);

	std::cout << c << '\n';


	Bicycle t(Wheel(20), Wheel(20), 300);

	std::cout << t << "\n\n";



	std::vector<Vehicle*> v;

	v.push_back(new Car(Engine(150), Wheel(17), Wheel(17), Wheel(18), Wheel(18), 250));

	//v.push_back(new Circle(Point(1, 2, 3), 7));

	v.push_back(new Car(Engine(200), Wheel(19), Wheel(19), Wheel(19), Wheel(19), 130));

	v.push_back(new WaterVehicle(5000));


	for (int i = 0; i < v.size(); i++)
		std::cout << *v[i] << "\n";

	std::cout << "\nThe highest power is " << getHighestPower(v) << '\n';

	for (int i = v.size() - 1; i > -1; i--) {
		Vehicle* tmp = v[i];
		//delete tmp;
		v.pop_back();
	}
	return 0;

}
