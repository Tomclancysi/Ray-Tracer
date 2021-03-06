#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

/**
 * @projectName   RayTracer
 * @brief         A signleton abstract class.
 * @author        YangWC
 * @date          2019-05-15
 */

namespace RayTracer
{

	template<typename T>
	class Singleton
	{
	private:
		Singleton(const Singleton<T> &) {}

		Singleton& operator=(const Singleton<T> &) {}

	protected:
		static std::shared_ptr<T> _instance;

	public:
		Singleton() = default;

		~Singleton() = default;

		static std::shared_ptr<T> getSingleton()
		{
			if (_instance == nullptr)
				_instance = std::make_shared<T>();
			return _instance;
		}
	};

}

#endif // SINGLETON_H
