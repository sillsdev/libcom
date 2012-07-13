#pragma once
#ifndef __ThreadLocalStorage_H__
#define __ThreadLocalStorage_H__

#include <pthread.h>

/* class ThreadLocalStorage implements TLS using the pthread library.
 *
*/
class ThreadLocalStorage
{
public:
	// default construtor
	inline ThreadLocalStorage();

	// sets the TLS data
	inline bool Set(const void *value);

	// gets the TLS data
	inline bool Get(void **value) const;

protected:
	// creates the TLS this is called by the construtor sets m_error if failes
	inline void Create();

protected:
	// The actual TLS data item.
	pthread_key_t m_luTls;

	// set to true or false by Create
	bool m_error;
};

/* ThreadLocalStorage::ThreadLocalStorage()
 * Default construtor that creates the TLS data
 */
inline ThreadLocalStorage::ThreadLocalStorage()
{
	// Create the pthread thread local storage.
	Create();
}

/* ThreadLocalStorage::Set
 * Set the thread local storage represented by the instance of this class
 */
inline bool ThreadLocalStorage::Set(const void* value)
{
	bool rv = false;
	if (!m_error)
	{
		if (pthread_setspecific(m_luTls, value) == 0)
			rv = true;
	}

	return rv;
}

/* ThreadLocalStorage::Get
 * Gets the thread local storage represented by the instance of this class
 * Only returns true if the TLS contains some data
*/
inline bool ThreadLocalStorage::Get(void** value) const
{
	bool rv = false;
	if (!m_error)
	{
		*value = pthread_getspecific(m_luTls);
		if (*value)
			rv = true;
	}

	return rv;
}

/* ThreadLocalStorage::Create
   creates the thread local storage data item.
 */
inline void ThreadLocalStorage::Create()
{
	// pthread_key_c reate returns 0 on success
	m_error = pthread_key_create(&m_luTls, 0);
}

#endif //__ThreadLocalStorage_H__
