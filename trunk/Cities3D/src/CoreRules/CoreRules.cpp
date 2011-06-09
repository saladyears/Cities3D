/*
 *  Cities3D - Copyright (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "AnySerializer.h"
#include "Player.h"
#include "ColorInfoData.h"
#include "IAnyEngine.h"
#include "define/defineHelper.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define REGISTER_STANDARD_SERIALIZER(T) \
namespace \
{ \
	struct sAny##T \
	{ \
		sAny##T(IAnySerializer *serializer) : mSerializer(serializer) \
		{ \
			ANYENGINE.AddAnySerializer(mSerializer); \
		} \
		~sAny##T() \
		{ \
			ANYENGINE.RemoveAnySerializer(mSerializer); \
		} \
	private: \
		IAnySerializerPtr mSerializer; \
	}; \
	sAny##T sObject##T(new AnySerializer<T>("AnySerializer<"#T">")); \
}

#define REGISTER_EXPLICIT_SERIALIZER(T) \
namespace \
{ \
	struct sAny##T \
	{ \
		sAny##T(IAnySerializer *serializer) : mSerializer(serializer) \
		{ \
			ANYENGINE.AddAnySerializer(mSerializer); \
		} \
		~sAny##T() \
		{ \
			ANYENGINE.RemoveAnySerializer(mSerializer); \
		} \
	private: \
		IAnySerializerPtr mSerializer; \
	}; \
	sAny##T sObject##T(new T(#T)); \
}

#define REGISTER_EXPLICIT_SERIALIZER_1(T, U) \
namespace \
{ \
struct sAny##T##U \
	{ \
	sAny##T##U(IAnySerializer *serializer) : mSerializer(serializer) \
		{ \
			ANYENGINE.AddAnySerializer(mSerializer); \
		} \
		~sAny##T##U() \
		{ \
			ANYENGINE.RemoveAnySerializer(mSerializer);\
		} \
	private: \
		IAnySerializerPtr mSerializer; \
	}; \
	sAny##T##U sObject##T##U(new T<U>(#T"<"#U">")); \
}

#define REGISTER_EXPLICIT_SERIALIZER_2(T, U, V) \
namespace \
{ \
struct sAny##T##U##V \
	{ \
	sAny##T##U##V(IAnySerializer *serializer) : mSerializer(serializer) \
		{ \
			ANYENGINE.AddAnySerializer(mSerializer); \
		} \
		~sAny##T##U##V() \
		{ \
			ANYENGINE.RemoveAnySerializer(mSerializer);\
		} \
	private: \
		IAnySerializerPtr mSerializer; \
	}; \
	sAny##T##U##V sObject##T##U##V(new T<U, V>(#T"<"#U", "#V">")); \
}

//---------------------------- STATICS          -----------------------------//
namespace
{
	class DateTimeSerializer : public AnySerializer<wxDateTime>
	{
	public:
		DateTimeSerializer(const HashString& type)
			: AnySerializer<wxDateTime>(type)
		{
		}

		virtual void Serialize(const boost::any &data, 
			wxDataOutputStream &output)
		{
			const wxDateTime *time = boost::any_cast<wxDateTime>(&data);

			if(NULL == time)
			{
				boost::throw_exception(boost::bad_any_cast());
			}

			saveDateTime(output, *time);
		}

		virtual void Unserialize(boost::any &data, wxDataInputStream &input)
		{
			wxDateTime time;
			loadDateTime(input, time);
			data = boost::any(time);
		}
	};

	class PlayerSerializer : public AnySerializer<Player>
	{
	public:
		PlayerSerializer(const HashString& type)
			: AnySerializer<Player>(type)
		{
		}

		virtual void Serialize(const boost::any &data, 
			wxDataOutputStream &output)
		{
			const Player *player = boost::any_cast<Player>(&data);

			if(NULL == player)
			{
				boost::throw_exception(boost::bad_any_cast());
			}

			output << *player;
		}

		virtual void Unserialize(boost::any &data, wxDataInputStream &input)
		{
			Player player;
			input >> player;
			data = boost::any(player);
		}
	};

	class ColorSerializer : public AnySerializer<ColorType>
	{
	public:
		ColorSerializer(const HashString& type)
			: AnySerializer<ColorType>(type)
		{
		}

		virtual void Serialize(const boost::any &data, 
			wxDataOutputStream &output)
		{
			const ColorType *color = boost::any_cast<ColorType>(&data);
			output << *color;
		}

		virtual void Unserialize(boost::any &data, wxDataInputStream &input)
		{
			wxInt32 color;

			input >> color;
			data = boost::any(static_cast<ColorType>(color));
		}
	};

	template <typename T>
	class ArraySerializer : public AnySerializer<std::vector<T> >
	{
	public:
		ArraySerializer(const HashString& type)
			: AnySerializer<std::vector<T> >(type)
		{
		}

		virtual void Serialize(const boost::any &data, 
			wxDataOutputStream &output)
		{
			const std::vector<T> *array = boost::any_cast<
				std::vector<T> >(&data);

			wxUint32 size = array->size();
			output << size;

			for(wxUint32 i = 0; i < size; ++i)
			{
				output << (*array)[i];
			}
		}

		virtual void Unserialize(boost::any &data, wxDataInputStream &input)
		{
			wxUint32 size;

			input >> size;
			std::vector<T> array;
			array.resize(size);

			for(wxUint32 i = 0; i < size; ++i)
			{
				input >> array[i];
			}

			data = boost::any(array);
		}
	};

	template <typename T, typename U>
	class MapSerializer : public AnySerializer<std::map<T, U> >
	{
	public:
		MapSerializer(const HashString& type)
			: AnySerializer<std::map<T, U> >(type)
		{
		}

		virtual void Serialize(const boost::any &data, 
			wxDataOutputStream &output)
		{
			const std::map<T, U>* map= boost::any_cast<std::map<T, U> >(&data);

			wxUint32 size = map->size();
			output << size;

			typename std::map<T, U>::const_iterator it, itEnd = map->end();
			for(it = map->begin(); it != itEnd; ++it)
			{
				output << it->first;
				output << it->second;
			}
		}

		virtual void Unserialize(boost::any &data, wxDataInputStream &input)
		{
			wxUint32 size;

			input >> size;
			std::map<T, U> map;

			for(wxUint32 i = 0; i < size; ++i)
			{
				T t;
				U u;
				input >> t;
				input >> u;
				map[t] = u;
			}

			data = boost::any(map);
		}
	};
}

//register the types we need
REGISTER_STANDARD_SERIALIZER(wxInt32);
REGISTER_STANDARD_SERIALIZER(wxString);
REGISTER_STANDARD_SERIALIZER(HashString);
REGISTER_STANDARD_SERIALIZER(bool);
REGISTER_EXPLICIT_SERIALIZER(DateTimeSerializer);
REGISTER_EXPLICIT_SERIALIZER(PlayerSerializer);
REGISTER_EXPLICIT_SERIALIZER(ColorSerializer);
REGISTER_EXPLICIT_SERIALIZER_1(ArraySerializer, wxInt32);
REGISTER_EXPLICIT_SERIALIZER_1(ArraySerializer, Player);
REGISTER_EXPLICIT_SERIALIZER_2(MapSerializer, HashString, wxInt32);

#ifdef __WIN32__
wxBOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}
#endif

//---------------------------- PUBLIC           -----------------------------//

//---------------------------------------------------------------------------//
// Topic: CoreRules
// 

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORT const RuleSetMap& ProvideRuleSets()
{
	return RuleSetDatabase::get().map();
}

DLL_EXPORT void ProvideModuleInfo(wxString& module, wxInt32& priority)
{
	module = stCore;
	priority = 0;
}

#ifdef __cplusplus
}
#endif

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//

