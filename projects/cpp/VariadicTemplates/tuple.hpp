#pragma once
namespace KB
{
	// source : https://habr.com/ru/post/228031/

	// only declaration
	template<typename... Args>
	struct tuple;

	// specialization
	template<typename Head, typename... Tail>
	struct tuple<Head, Tail...> : tuple<Tail...>
	{
		tuple(Head h, Tail... tail) : tuple<Tail...>(tail...), head(h) {}
		typedef tuple<Tail...> base_type;
		typedef Head           value_type;

		base_type& next = static_cast<base_type&>(*this);
		Head       head;
	};

	// specialization
	template<>
	struct tuple<>
	{};

	////////////////////////////////////////////////////////////////////////

	template<int position, typename Head, typename... Args>
	struct getter
	{
		typedef typename getter<position - 1, Args...>::return_type return_type;
		static return_type get(tuple<Head, Args...> t)
		{
			return getter<position - 1, Args...>::get(t);
		}
	};

	template<typename Head, typename... Args>
	struct getter<0, Head, Args...>
	{
		typedef typename tuple<Head, Args...>::value_type return_type;
		static return_type get(tuple<Head, Args...> t)
		{
			return t.head;
		}
	};

	////////////////////////////////////////////////////////////////////////

	template<int position, typename Head, typename... Args>
	typename getter<position, Head, Args...>::return_type
		get(tuple<Head, Args...> t)
	{
		return getter<position, Head, Args...>::get(t);
	}
}