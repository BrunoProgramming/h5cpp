#include <iostream>
#include <blitz/array.h>
#include <h5cpp/all>

using namespace std;
// 

template<class T> using Colvec = blitz::Array<T,1>;
template<class T> using Matrix = blitz::Array<T,2>;
template<class T> using Cube   = blitz::Array<T,3>;

int main(){
	{ // CREATE - WRITE
		Matrix<double> M(4,5); 				// create a matrix
		hid_t fd = h5::create("blitz.h5"); 	// and a file
			h5::write(fd,"blitz/M",M); 		// save dataset
		h5::close(fd); 						// close fd
	}
	{ // CREATE - READ 
		hid_t fd = h5::open("blitz.h5", H5F_ACC_RDWR); 				// you can have multiple fd open with H5F_ACC_RDONLY, but single write
			hid_t ds = h5::create<double>(fd,"dataset",{3,2});		// create dataset, default to NaN-s
			auto M = h5::read<Matrix<double>>( ds ); 				// read data back as matrix
			std::cout << M <<std::endl;
		h5::close(fd); 												// close fd
	}
	{ // READ
		Matrix<double> M = h5::read<Matrix<double>>("blitz.h5","blitz/M"); 	// read entire dataset back with a single read
		std::cout << M<<std::endl;
	}
	{
		// PARTIAL READ
		hid_t fd = h5::create("blitz-partial.h5");
		Colvec<float> data(5);  for( int i=0; i<5; i++ ) data(i) = i;
		hid_t ds = h5::create<float>(fd,"/blitz/data",{5,5,1,2});

		// offset - where to start within the array, in this case is {0,0,0,0}
		// count  - how many elements we would like to write: {a,b,c} 
		h5::write(ds, data, {0,0,0,0},{1,5,1,1} ); 			//
		// we only want 3 elements back from offset 1, the rank must be 4
		auto V  = h5::read<Colvec<double>>(ds,{0,1,0,0},{1,3,1,1});
		std::cout << V << std::endl;

		H5Dclose(ds);
		h5::close(fd);
	}
}



