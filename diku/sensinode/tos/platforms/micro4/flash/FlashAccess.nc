/*
 * Copyright (c) 2007 University of Copenhagen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of University of Copenhagen nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY
 * OF COPENHAGEN OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *
 * @author Marcus Chang <marcus@diku.dk>
 * @author Klaus S. Madsen <klaussm@diku.dk>
 */


interface FlashAccess {

  command uint16_t firstUsablePage();
  command uint16_t lastUsablePage();

  /**
   * Read a page from the flash. 
   *
   * @param page_no The page to read from.
   * @param page    A buffer to hold the page bring read.
   * @return SUCCESS, the page could be read
   */
	command error_t read(uint16_t page_no, void *page);
	event void readReady(uint16_t page_no, void *page, uint16_t length);

  /**
   * Erase the sector holding the page in the flash.
   *
   * @param page_no The page inside the sector.
   * @return SUCCESS, the sector was erased.
   */
  command error_t erase(uint16_t page_no);
  event void eraseDone(uint16_t page_no);

  /**
   * Erase the entire flash.
   *
   * @return SUCCESS, the flash was erased.
   */
  command error_t eraseAll();
  event void eraseAllDone();

  /**
   * Write a page to the flash
   *
   * <p>Note this function does not clear the page before the
   * write. This must be done by calling erase</p>
   *
   * @param page_no The page to write
   * @param page    A buffer containing the contents to write
   * @result SUCCESS, the self-programming has successfully finished.
   */
  command error_t write(uint16_t page_no, void *page); 
  event void writeDone(uint16_t page_no, void *page);

}
