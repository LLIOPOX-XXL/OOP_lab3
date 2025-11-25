// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>

class ProgramError : public std::exception {
public:
	const char* what() const noexcept override {
		return "Programm Error";
	}
};

class InsufficientPrivilegesError : public ProgramError {
public:
	const char* what() const noexcept override {
		return "Insufficient priveleges error";
	}
};
class ConversionError : public ProgramError {
public:
	const char* what() const noexcept override {
		return "Conversion Error";
	}
};
class UnableToConvertError : public ConversionError {
public:
	const char* what() const noexcept override {
		return "Conversion Error: Unable to convert this value";
	}
};
class UnableToCastInterface : public ConversionError {
public:
	const char* what() const noexcept override {
		return "Unable to cast to interface";
	}
};


template<typename ErrType>
class ProgErrorWrapper : public std:: exception {
private:
	std::shared_ptr<ErrType> err;

public:
	ProgErrorWrapper(std::shared_ptr<ErrType> err) : err(err) {}
	const char* what() const noexcept override {
		return err->what();
	}
};



template<typename ErrType>
class ErrContainer {
private:
	std::vector<std::shared_ptr<ErrType>> container;
	int len = 0;

public:
	template<typename ErrName>
	void add(const ErrName& err) {
		container.push_back(std::make_shared<ErrName>(err));
		len += 1;
	}
	int length() { return len;  }

	std::shared_ptr<ErrType>& operator[](unsigned long index) {
		return container[index];
	}
};

int main()
{
	ErrContainer<ProgramError> errors;
	errors.add(InsufficientPrivilegesError());
	errors.add(UnableToCastInterface());
	errors.add(ConversionError());
	errors.add(UnableToConvertError());
	//errors.push_back(std::make_shared<ProgErrorWrapper>(std::make_shared<InsufficientPrivilegesError>(InsufficientPrivilegesError())));
	
	for (int i = 0; i < errors.length(); i++) {
		try {
			throw errors[i];
		}
		catch (const std::shared_ptr<ProgramError>& err) {
			std::cout << "Error: " << err->what() << std::endl;
		}
	}
}
