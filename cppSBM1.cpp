#include <iostream>
#include <stdarg.h>
#include <string>

enum class Funcs {
	LINEAR
	, RELU
	, SIGN
	, TANH
};

class Matrix {
private:
	float*** weights;
	float** data;
	float** summas;
	int* sizes;
	int count;
	float (*(*funcs))(float x) ;
	float (*(*dfuncs))(float x) ;
	bool inited = false;
	float nu = 0.001;

	float* deltas;
	float* ddeltas;

	static float linear(float x) { return x; }
	static float dlinear(float x) { return 1; }
	static float relu(float x) { return x < 0 ? 0.01 * x : x; }
	static float drelu(float x) { return x >= 0; }
	static float sign(float x) { return 1 / (1 + exp(-x)); }
	static float dsign(float x) { float f = sign(x); return f * (1 - f); }
	static float tanh(float x) { return (exp(x) - exp(-x)) / (exp(x) + exp(-x)); }
	static float dtanh(float x) { float f = tanh(x); return 1 - f * f; }

	void InitData() {
		data = new float* [count];
		summas = new float* [count];
		weights = new float** [count - 1];
		int max = 0;
		for (int i = 0; i < count; i++) {
			data[i] = new float[sizes[i]];
			summas[i] = new float[sizes[i]];
			if (max < sizes[i]) max = sizes[i];
			if (i > 0) {
				weights[i - 1] = new float* [sizes[i]];
				for (int j = 0; j < sizes[i]; j++)
					weights[i - 1][j] = new float[sizes[i - 1] + 1];
			}
		}
		deltas = new float[max + 1];
		ddeltas = new float[max + 1];
	}
	void _front() {
		/*for (int i = 0; i < sizes[0]; i++)
			summas[0][i] = data[0][i];*/
		for (int i = 1; i < count; i++) 
			for (int j = 0; j < sizes[i]; j++) {
				float sum = 0;
				for (int k = 0; k < sizes[i - 1]; k++) {
					sum += data[i - 1][k] * weights[i - 1][j][k];
				}

				summas[i][j] = sum + weights[i - 1][j][sizes[i - 1]];
				data[i][j] = funcs[i-1](summas[i][j]);
			}

	}

	float _back(float* real) {
		float err = 0;
		for (int i = 0; i < sizes[count - 1]; i++) {
			float e = data[count - 1][i] - real[i];
			err += e * e;
			deltas[i] = e * dfuncs[count - 2](summas[count - 1][i]);
		}
		err = sqrt(err / sizes[count - 1]);
		for (int i = count - 1; i > 1; i--) {
			if (i > 2) {
				for (int j = 0; j < sizes[i - 1] + 1; j++) {
					ddeltas[j] = 0;
					for (int k = 0; k < sizes[i]; k++)
						ddeltas[j] += weights[i - 1][k][j] * deltas[k];
					ddeltas[j] *= dfuncs[i - 2]( j == sizes[i - 1] ? 1 : summas[i - 1][j]);
				}
			}
			for (int j = 0; j < sizes[i]; j++) {
				for (int k = 0; k < sizes[i - 1] + 1; k++) {
					weights[i - 1][j][k] -= nu * deltas[j] * (k == sizes[i - 1] ? 1 : data[i - 1][k]);
				}
			}
			for (int j = 0; j < sizes[i - 1] + 1; j++) {
				deltas[j] = ddeltas[j];
			}
		}
		return err;
	}

	float _learn(int count, float** xs, float** ys) {
		float err = 0;
		for (int i = 0; i < count; i++) {
			delete[] Front(xs[i]);
			err += _back(ys[i]);
		}
		return err / count;
	}
public:
	void SetNu(float n) {
		nu = n;
	}
	void prnt() {
		for (int i = 0; i < count; i++) {
			std::cout << "\n";
			for (int j = 0; j < sizes[i]; j++)
				std::cout << data[i][j] << "\t";
		}

		std::cout << "\n";
	}
	~Matrix() {
		std::cout << "Deleting matrix\n";
		delete[] weights;
		delete[] data;
		delete[] summas;
		delete[] sizes;
		delete[] funcs;
		delete[] dfuncs;
		delete[] deltas;
		delete[] ddeltas;
	}
	Matrix() {
		std::cout << "Creating matrix\n";
		inited = false;
		count = 0;
		sizes = nullptr;
		funcs = nullptr;
		dfuncs = nullptr;
		weights = nullptr;
		data = nullptr;
		summas = nullptr;
		deltas = nullptr;
		ddeltas = nullptr;
	}
	Matrix(const Matrix& other) {
		std::cout << "Creating copy matrix\n";
		summas = nullptr;
		deltas = nullptr;
		ddeltas = nullptr;
		if (other.inited) {
			count = other.count;
			inited = true;
			sizes = new int[count];
			funcs = new (float (*[count - 1])(float));
			dfuncs = new (float (*[count - 1])(float));
			for (int i = 0; i < count - 1; i++) {
				funcs[i] = other.funcs[i];
				dfuncs[i] = other.dfuncs[i];
				sizes[i] = other.sizes[i];
			}
			sizes[count - 1] = other.sizes[count - 1];
			InitData();

			for (int i = 1; i < count; i++) 
				for (int j = 0; j < sizes[i]; j++)
					for (int k = 0; k < sizes[i - 1] + 1; k++)
						weights[i - 1][j][k] = other.weights[i - 1][j][k];
			
		}
		else {
			inited = false;
			count = 0;
			sizes = nullptr;
			funcs = nullptr;
			dfuncs = nullptr;
			weights = nullptr;
			data = nullptr;
		}
	}
	Matrix operator=(const Matrix& other) {
		std::cout << "Copy matrix\n";
		if (inited) {
			std::cout << "Matrix already inited!\n";
		}
		else if (other.inited) {
			count = other.count;
			inited = true;
			sizes = new int[count];
			funcs = new (float (*[count - 1])(float));
			dfuncs = new (float (*[count - 1])(float));
			for (int i = 0; i < count - 1; i++) {
				funcs[i] = other.funcs[i];
				dfuncs[i] = other.dfuncs[i];
				sizes[i] = other.sizes[i];
			}
			sizes[count - 1] = other.sizes[count - 1];
			InitData();

			for (int i = 1; i < count; i++)
				for (int j = 0; j < sizes[i]; j++)
					for (int k = 0; k < sizes[i - 1] + 1; k++)
						weights[i - 1][j][k] = other.weights[i - 1][j][k];
		}
		return *this;
	}

	void Init(int count, ...) {
		if (!inited) {
			inited = true;
			va_list vl;
			va_start(vl, count);

			this->count = count;
			sizes = new int[count];
			funcs = new (float (*[count-1])(float));
			dfuncs = new (float (*[count-1])(float));

			sizes[0] = va_arg(vl, int);
			for (int i = 1; i < count; i++) {
				Funcs fun = va_arg(vl, Funcs);
				switch (fun)
				{
				case Funcs::LINEAR:
					funcs[i - 1] = linear;
					dfuncs[i - 1] = linear;
					break;
				case Funcs::RELU:
					funcs[i - 1] = relu;
					dfuncs[i - 1] = drelu;
					break;
				case Funcs::SIGN:
					funcs[i - 1] = sign;
					dfuncs[i - 1] = dsign;
					break;
				case Funcs::TANH:
					funcs[i - 1] = tanh;
					dfuncs[i - 1] = dtanh;
					break;
				default:
					funcs[i - 1] = linear;
					dfuncs[i - 1] = linear;
					break;
				}
				sizes[i] = va_arg(vl, int);
			}
		
			va_end(vl);
			InitData();
			RandomizeWeights();
		}
		else {
			std::cout << "Object already inited!\n";
		}
	}

	void RandomizeWeights() {
		for (int i = 1; i < count; i++) {
			for (int j = 0; j < sizes[i]; j++)
				for (int k = 0; k < sizes[i - 1] + 1; k++)
					weights[i - 1][j][k] = float(rand()) / RAND_MAX * 4 - 2;
		}
	}

	float* Front(float* x) {
		if (!inited) {
			std::cout << "Front called but matrix not inited!\n";
			return nullptr;
		}
		for (int i = 0; i < sizes[0]; i++)
			data[0][i] = x[i];
		_front();
		float* res = new float[sizes[count - 1]];
		for (int i = 0; i < sizes[count - 1]; i++)
			res[i] = data[count - 1][i];
		return res;
	}

	void Learn(int count, float** xs, float** ys, int eps = 1000, int inep = 30) {
		float** xxs = new float*[inep];
		float** yys = new float*[inep];
		for (int i = 0; i < eps; i++) {
			float err = 0;
			for (int j = 0; j < inep; j++) {
				int n = rand() % count;
				xxs[j] = xs[n];
				yys[j] = ys[n]; 
			}
			std::cout << i << " probe: " << _learn(inep, xxs, yys) << "\n";
			if (i == 602 || i == 603 || i == 605)
				prnt();

		}
		for (int j = 0; j < inep; j++) {
			xxs[j] = nullptr;
			yys[j] = nullptr;
		}
		delete[] xxs;
		delete[] yys;
	}
	//void Learn(int count, float** xs, float** ys, int eps = 1000, int inep = 30) {
	//	float** xxs = new float*[inep];
	//	float** yys = new float*[inep];
	//	for (int i = 0; i < eps; i++) {
	//		float err = 0;
	//		/*for (int j = 0; j < inep; j++) {
	//			int n = rand() % count;
	//			xxs[j] = xs[n];
	//			yys[j] = ys[n]; _learn(inep, xxs, yys)
	//		}*/
	//		std::cout << i << " probe: " << _learn(count, xs, ys) << "\n";
	//		/*if (i == 602 || i == 603 || i == 605)
	//			prnt();*/

	//	}
	//	for (int j = 0; j < inep; j++) {
	//		xxs[j] = nullptr;
	//		yys[j] = nullptr;
	//	}
	//	delete[] xxs;
	//	delete[] yys;
	//}
};

float func(float x, float y, float z) {
	return y * (3 * x * x + 2 * y * x * z - z * z * 6) / 14;
		//2 * x - 10 * y + z - 4;
		// 2 * x + y * y - x * z;
}

void get_diapasone(float (*f)(float, float, float), float  xmin, float xmax, float ymin, float ymax, float  zmin, float zmax, float& fmin, float& fmax) {
	fmin = fmax = f(xmin, ymin, zmin);
	for (float i = xmin; i < xmax; i += (xmax - xmin) / 100)
		for (float j = ymin; j < ymax; j += (ymax - ymin) / 100)
			for (float k = zmin; k < zmax; k += (zmax - zmin) / 100) {
				float r = f(i, j, k);
				if (r < fmin) fmin = r;
				if (r > fmax) fmax = r;
			}
}
float normalize(float val, float min, float max) {
	return (val - (min + max) / 2) / (max - min) * 2;
}
float denormalize(float val, float min, float max) {
	return val / 2 * (max - min) + (min + max) / 2;
}


#define SIZE_LEARN 10000
#define SIZE_CHECK 20
#define COUNT_ITERS 500

void check(Matrix& m, float ** xs, float ** ys, float x_, float xu, float y_, float yu, float z_, float zu, float f_, float fu) {
	std::cout << "\n\n";
	float err = 0;
	for (int i = 0; i < SIZE_CHECK; i++) {
		float x = denormalize(xs[i][0], x_, xu),
			y = denormalize(xs[i][1], y_, yu),
			z = denormalize(xs[i][2], z_, zu),
			f;
		float* tmp = m.Front(xs[i]);
		f = denormalize(tmp[0], f_, fu);
		delete[] tmp;

		std::cout << "func(" << x;
		std::cout << ", " << y;
		std::cout << ", " << z;
		std::cout << ")  \t= " << f << "\t from " << func(x, y, z) << "\t\t\tErr: " << abs(f - func(x, y, z)) << "\n";
		err += abs(f - func(x, y, z)) * abs(f - func(x, y, z));
	}
	std::cout << "MSE: " << sqrt(err / SIZE_CHECK) << "\n";
}




int main()
{

	Matrix m;

	// ----------------------------------------------------- INITIALIZATION AND FIT -------------------------------------------- //
	m.SetNu(0.01);
	//m.Init(5, 2, Funcs::TANH, 4, Funcs::TANH, 5, Funcs::TANH, 4, Funcs::TANH, 1);
	m.Init(5, 3, Funcs::TANH, 15, Funcs::TANH, 12, Funcs::TANH, 12, Funcs::TANH, 1);

	float	x_ = -10, xu = 10,
			y_ = -1, yu = 2,
			z_ = -5, zu = 8;
	float f_ = 0, fu = 0;
	get_diapasone(func, x_, xu, y_, yu, z_, zu, f_, fu);

	float dt[6]{ x_, xu, y_, yu, z_, zu };

	float** xs = new float* [SIZE_LEARN];
	float** ys = new float* [SIZE_LEARN];

	for (int i = 0; i < SIZE_LEARN; i++){
		xs[i] = new float[3];
		for (int j = 0; j < 3; j++)
			xs[i][j] = denormalize((float(rand()) / RAND_MAX * 2 - 1), dt[j * 2], dt[j * 2 + 1]);
			//xs[i][j] = (float(rand()) / RAND_MAX) * (dt[j * 2 + 1] - dt[j * 2]) + (dt[j * 2 + 1] + dt[j * 2]) / 2;
		ys[i] = new float[1]{ normalize(func(xs[i][0], xs[i][1], xs[i][2]), f_, fu) };
		for (int j = 0; j < 3; j++)
			xs[i][j] = normalize(xs[i][j], dt[j * 2], dt[j * 2 + 1]);
	}

	m.Learn(SIZE_LEARN, xs, ys, COUNT_ITERS, SIZE_LEARN / 3);

	delete[] xs;
	delete[] ys;


	std::cout << std::fixed;
	std::cout.precision(2);

	std::cout << "\n\n";
	std::cout << "x: (" << x_ << ":" << xu << ")\n";
	std::cout << "y: (" << y_ << ":" << yu << ")\n";
	std::cout << "z: (" << z_ << ":" << zu << ")\n";
	std::cout << "f: (" << f_ << ":" << fu << ")\n";



	// ----------------------------------------------------- SET CHECKS VARS -------------------------------------------- //
	xs = new float* [SIZE_CHECK];
	ys = new float* [SIZE_CHECK];

	for (int i = 0; i < SIZE_CHECK; i++) {
		xs[i] = new float[3];
		for (int j = 0; j < 3; j++)
			xs[i][j] = denormalize((float(rand()) / RAND_MAX * 2 - 1), dt[j * 2], dt[j * 2 + 1]);
		ys[i] = new float[1]{ normalize(func(xs[i][0], xs[i][1], xs[i][2]), f_, fu) };
		for (int j = 0; j < 3; j++)
			xs[i][j] = normalize(xs[i][j], dt[j * 2], dt[j * 2 + 1]);
	}

	
	// ----------------------------------------------------- LOOK WHERE -------------------------------------------- //
	check(m, xs, ys, x_, xu, y_, yu, z_, zu, f_, fu);
	Matrix m2 = m;
	Matrix m3;
	m3 = m2;
	check(m2, xs, ys, x_, xu, y_, yu, z_, zu, f_, fu);
	check(m3, xs, ys, x_, xu, y_, yu, z_, zu, f_, fu);


	delete[] xs;
	delete[] ys;

}
