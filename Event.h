#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <functional>

template<class... Args>
class Event {
public:
	using Function = std::function<void(Args...)>;
	using Container = std::vector<Function>;

	Container listeners;

	void operator()(Args... args) {
		for (auto& f : listeners)
			f(args...);
	}

	template<class T>
	Event& operator+=(T& f) {
		listeners.push_back(f);
		return *this;
	}
	
	template<class T>
	Event& operator+=(T&& f) {
		listeners.emplace_back(std::move(f));
		return *this;
	}

	Event& operator+=(Event& f) {
		listeners.push_back(std::ref(f));
		return *this;
	}
};

#endif // !EVENT_H
