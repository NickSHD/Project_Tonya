#include <iostream>
#include <map>

//дл€ создани€ класса реализуем абстрактную фабрику, как один из главных паттернов дл€ создани€ абстрактных классов
//приведена реализаци€ на шаблонах, потому что помимо класса добавки, можно добавить еще класс, например, sugar
//со своими наследниками и тд
template < class Base >
class AbstractCreator { //класс, от которого будет наследоватьс€ класс с нужным нам интерфейсом
public:
	AbstractCreator() {}
	virtual ~AbstractCreator() {}
	virtual Base* Create() const = 0;
};

template < class HeirClass, class Base >
class Creator : public AbstractCreator < Base > { //в этом классе и реализован метод, который по второму шаблону
												  //будет запускать конструктор нужного нам наследника
public:
	Creator() {}
	virtual ~Creator() {}

	Base* Create() const {
		return new HeirClass();
	}
};


template < class Base, class Type >
class ObjectFactory {//в этом классе реализована функци€ создани€ указател€ на абстрактный класс
protected:
	typedef AbstractCreator < Base > AbstractFactory;
	typedef std::map < Type, AbstractFactory* > FactoryMap;//будем хранить map, где первое поле индетификатор, а второе нужный нам конструктор
	FactoryMap Factory;

	void RegisterClass(const Type& id, AbstractFactory* temp) {//приватна€ функци€ дл€ удалени€ конструктора, конструктор же представл€ет из себ€ класс
		FactoryMap::iterator it = Factory.find(id);            //абстрактный, в наследнике, которого реализована нужна€ нам функци€ create
		if (it == Factory.end()) {
			Factory[id] = temp;
		}
		else
			delete temp;
	}

public:
	ObjectFactory() {}
	virtual ~ObjectFactory() {};

	template < class HeirClass >
	void add(const Type& id) {
		RegisterClass(id, new Creator < HeirClass, Base >());
	}

	Base* CreateObject(const Type& id) {
		FactoryMap::iterator it = Factory.find(id);
		if (it != Factory.end())
			return Factory[id]->Create();
		else
			return 0;
	}
};

class additive { //виртуальный класс, от которого наследуютс€ классы milk и coffee

protected:
	double Mass;

public:
	virtual void show() = 0; //объ€вление виртуальной функции
	
};

class milk :public virtual additive { // здесь, как и в классе coffee будем реализовывать виртуальное наследование
									  //чтобы решить проблему ромбовидного наследовани€
protected:

	bool isCream;

public:
	milk(bool Cream, double mMass) {
		Mass = mMass;
		isCream = Cream;
	}

	milk(const milk &a) {
		isCream = a.isCream;
		Mass = a.Mass;
	}
	void show() {
		std::cout << "Milk:\nCream " << isCream << "\nMass=" << Mass << std::endl;
	}
	~milk() {}
};


class coffee : virtual public additive{

protected:

	bool isArabica;

public:
	coffee(bool Arabica, double cMass) {
		isArabica = Arabica;
		Mass = cMass;
	}

	coffee(const coffee &a) {
		isArabica = a.isArabica;
		Mass = a.Mass;
	}
	void show() {
		std::cout << "Coffee:\nArabica " << isArabica << "\nMass=" << Mass << std::endl;
	}
	~coffee() {}
};


class drink : public milk, public coffee {

protected:
	double sugarMass;

public:
	drink(bool Arabica, double cMass, bool Cream, double mMass, double sMass) :
		coffee(Arabica, cMass), milk(Cream, mMass), sugarMass(sMass) {}
	drink(const coffee &c, const milk &m, double sMass) :coffee(c), milk(m) {
		sugarMass = sMass;
	}
	drink(const drink &c) :coffee(c.isArabica, c.Mass), milk(c.isCream, c.Mass) {
		sugarMass = c.sugarMass;
	}
	void show() {
		std::cout << "\tDrink:\n";
		milk::show(); coffee::show();
		std::cout << "Sugar:" << sugarMass << "g.\n";
	}
	drink operator++() {
		sugarMass += 50.;
		return *this;
	}
	~drink() {}
};

additive* make_additive(const std::string& id) {
	ObjectFactory < additive, std::string >* ConcreteFactory = new ObjectFactory < additive, std::string >();
	ConcreteFactory->add< milk >("milk");
	ConcreteFactory->add< coffee >("coffee");
	return ConcreteFactory->CreateObject(id);
}

void main()
{
	milk m0(true, 15), m1(m0);
	m0.show(); m1.show();
	std::cout << std::endl;
	coffee c0(false, 40), c1(c0);
	c0.show(); c1.show();
	std::cout << std::endl;
	drink d0(true, 65.9, false, 10.1, 5), d1(c0, m0, 1.5), d2(d1);
	++d2;
	d0.show(); d1.show(); d2.show();
	std::cout << std::endl;
	system("pause");
};