/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2009
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

namespace Gecode { namespace Support {

  /*
   * Thread
   */
  forceinline
  Thread::~Thread(void) {
  }


  /*
   * Mutex
   */
  forceinline void
  Mutex::acquire(void) {
    if (pthread_mutex_lock(&p_m) != 0)
      throw OperatingSystemError("Mutex::acquire[pthread_mutex_lock]");
  }
  forceinline bool
  Mutex::tryacquire(void) {
    return pthread_mutex_trylock(&p_m) == 0;
  }
  forceinline void
  Mutex::release(void) {
    if (pthread_mutex_unlock(&p_m) != 0)
      throw OperatingSystemError("Mutex::release[pthread_mutex_unlock]");
  }


  /*
   * Event
   */
  forceinline void
  Event::signal(void) {
    if (pthread_mutex_lock(&p_m) != 0)
      throw OperatingSystemError("Event::signal[pthread_mutex_lock]");
    if (!p_s) {
      p_s = true;
      if (p_w && (pthread_cond_signal(&p_c) != 0))
        throw OperatingSystemError("Event::signal[pthread_cond_signal]");
    }
    if (pthread_mutex_unlock(&p_m) != 0)
      throw OperatingSystemError("Event::signal[pthread_mutex_unlock]");
  }
  forceinline void
  Event::wait(void) {
    if (pthread_mutex_lock(&p_m) != 0)
      throw OperatingSystemError("Event::wait[pthread_mutex_lock]");
    if (!p_s) {
      p_w = true;
      if (pthread_cond_wait(&p_c,&p_m) != 0)
        throw OperatingSystemError("Event::wait[pthread_cond_wait]");
      p_w = false;
      assert(p_s);
    }
    p_s = false;
    if (pthread_mutex_unlock(&p_m) != 0)
      throw OperatingSystemError("Event::wait[pthread_mutex_unlock]");
  }


}}

// STATISTICS: support-any
