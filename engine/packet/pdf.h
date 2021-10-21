
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file packet/pdf.h
 *  \brief A packet that contains a PDF document.
 */

#ifndef __REGINA_PDF_H
#ifndef __DOXYGEN
#define __REGINA_PDF_H
#endif

#include <cstdlib>
#include <cstring>
#include "regina-core.h"
#include "packet/packet.h"

namespace regina {

class PDF;

/**
 * A packet that can hold a PDF document.
 *
 * This packet may or may not contain a PDF document at any given time.
 * This can be tested by calling isNull(), and can be changed by calling
 * reset().
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps; however,
 * these operations only copy/swap the mathematical content, not the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * \ingroup packet
 */
class PDF : public Packet {
    REGINA_PACKET(PACKET_PDF, "PDF")

    public:
        /**
         * Describes how a PDF packet should claim ownership of a block
         * of binary data.
         *
         * \ifacespython Not present.
         */
        enum OwnershipPolicy {
            /**
             * The packet should claim ownership of the block, and
             * should assume that it was allocated using \a malloc().
             */
            OWN_MALLOC,
            /**
             * The packet should claim ownership of the block, and
             * should assume that it was allocated using \c new[].
             */
            OWN_NEW,
            /**
             * The packet should not claim ownership of the block, but
             * should instead make its own deep copy.
             */
            DEEP_COPY
        };

    private:
        char* data_;
            /**< The binary data of the PDF document that is stored in this
                 packet, or \c null if no PDF document is currently stored. */
        size_t size_;
            /**< The size of the binary data in bytes, or 0 if no PDF
                 document is currently stored. */
        OwnershipPolicy alloc_;
            /**< Describes how the binary data (if any) was allocated; this
                 must be either \a OWN_MALLOC or \a OWN_NEW. */

    public:
        /**
         * Creates a PDF packet with no document stored.
         */
        PDF();

        /**
         * Creates a PDF packet with data read from the given PDF file.
         *
         * This routine does not check whether the given file \e looks
         * like a PDF document; it simply loads the file contents blindly.
         *
         * If the file could not be read or is empty, then no PDF document will
         * be stored in this packet.  You can test this by calling isNull().
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.
         *
         * @param filename the filename of the PDF document to read.
         */
        PDF(const char* filename);

        /**
         * Creates a packet to store the given PDF data.
         *
         * The \a data array must contain a full PDF document as a
         * block of binary data.
         *
         * The \a alloc argument shows if/how this packet claims ownership of
         * the data.  In particular, unless \a alloc is \a DEEP_COPY, this
         * packet will claim ownership of the given data block and will
         * deallocate it when the packet is destroyed.  If \a alloc is
         * \a DEEP_COPY then the given block of data will not be modified in
         * any way.
         *
         * It is possible to pass a null pointer as the data array, in
         * which case the new packet will have no PDF document stored
         * (so isNull() will return \c true).
         *
         * \ifacespython Not present.
         *
         * @param data the block of binary data that forms the PDF document,
         * or \c null if no document is to be stored.
         * @param size the number of bytes in this block of binary data;
         * if actual data is passed (i.e., \a data is not \c null) then
         * this must be strictly positive.
         * @param alloc describes if/how this packet should claim ownership
         * of the given block of data; see the notes above for details.
         */
        PDF(char* data, size_t size, OwnershipPolicy alloc);

        /**
         * Creates a new copy of the given PDF packet.
         *
         * Like all packet types, this only copies the PDF content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * it will not clone the given packet's children, and it will not
         * insert the new packet into any packet tree).
         *
         * This is safe to call even if \a src does not contain a PDF document.
         *
         * @param src the PDF packet whose contents should be copied.
         */
        PDF(const PDF& src);

        /**
         * Destroys this PDF packet and deallocates data if required.
         */
        ~PDF();

        /**
         * Sets this to be a copy of the given PDF packet.
         *
         * Like all packet types, this only copies the PDF content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * or change this packet's location in any packet tree).
         *
         * This is safe to call even if \a src does not contain a PDF document.
         *
         * @param src the PDF packet whose contents should be copied.
         * @return a reference to this packet.
         */
        PDF& operator = (const PDF& src);

        /**
         * Swaps the contents of this and the given PDF packet.
         *
         * Like all packet types, this only swaps the PDF content, not
         * the packet infrastructure (e.g., it will not swap packet labels,
         * or change either packet's location in any packet tree).
         *
         * This is safe to call even if this packet and/or \a other does not
         * contain a PDF document.
         *
         * @other the PDF packet whose contents should be swapped with this.
         */
        void swap(PDF& other);

        /**
         * Determines whether this packet is currently holding a PDF
         * document.
         *
         * @return \c true if and only if this packet is holding a
         * PDF document.
         */
        bool isNull() const;

        /**
         * Returns a pointer to the block of raw data that forms this
         * PDF document.  The number of bytes in this block can be found
         * by calling size().
         *
         * If no PDF document is currently stored (i.e., isNull()
         * returns \c true), then this routine will return a null pointer.
         *
         * \ifacespython Not present.
         *
         * @return the raw PDF data.
         */
        const char* data() const;

        /**
         * Returns the size of this PDF document in bytes.
         *
         * If no PDF document is currently stored (i.e., isNull()
         * returns \c true), then this routine will return zero.
         *
         * @return the number of bytes.
         */
        size_t size() const;

        /**
         * Empties this PDF packet so that no document is stored.
         * After calling this routine, isNull() will return \c true.
         *
         * The old data will be deallocated if required.
         */
        void reset();

        /**
         * Refills this PDF packet with the given PDF data.
         * The old data will be deallocated if required.
         *
         * This routine behaves like the class constructor; see the
         * constructor documentation for details.
         *
         * \ifacespython Not present.
         *
         * @param data the block of binary data that forms the new PDF
         * document, or \c null if no document is to be stored.
         * @param size the number of bytes in this new block of binary data;
         * if actual data is passed (i.e., \a data is not \c null) then
         * this must be strictly positive.
         * @param alloc describes if/how this packet should claim ownership
         * of the given block of data; see the notes above for details.
         */
        void reset(char* data, size_t size, OwnershipPolicy alloc);

        /**
         * Saves this PDF document to the given file in PDF format.
         *
         * This routine does not check whether the contents of this
         * packet \e look like a PDF document; it simply writes them
         * blindly to the filesystem.
         *
         * If no PDF document is currently stored in this PDF packet
         * (i.e., isNull() returns \c true), then this routine will do
         * nothing and simply return \c false.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it unchanged to low-level C/C++ file I/O routines.
         *
         * @param filename the name of the PDF file to write.
         * @return \c true if the file was successfully written, or
         * \c false otherwise.
         */
        bool savePDF(const char* filename) const;

        void writeTextShort(std::ostream& out) const override;

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const override;
};

/**
 * Swaps the contents of the given PDF packets.
 *
 * This global routine simply calls PDF::swap(); it is provided so that
 * PDF meets the C++ Swappable requirements.
 *
 * @param a the first PDF packet whose contents should be swapped.
 * @param b the second PDF packet whose contents should be swapped.
 *
 * \ingroup packet
 */
void swap(PDF& a, PDF& b);

// Inline functions for PDF

inline PDF::PDF() : data_(nullptr), size_(0), alloc_(OWN_NEW) {
}

inline PDF::PDF(char* data, size_t size, OwnershipPolicy alloc) :
        data_(data), size_(size), alloc_(alloc) {
    if (alloc_ == DEEP_COPY) {
        if (data_) {
            data_ = static_cast<char*>(::malloc(size_));
            ::memcpy(data_, static_cast<const char*>(data), size_);
        }
        alloc_ = OWN_MALLOC;
    } else if (! data_)
        size_ = 0;
}

inline PDF::PDF(const PDF& src) : PDF(src.data_, src.size_, DEEP_COPY) {
}

inline PDF::~PDF() {
    if (data_) {
        if (alloc_ == OWN_MALLOC)
            ::free(data_);
        else
            delete[] data_;
    }
}

inline PDF& PDF::operator = (const PDF& src) {
    reset(src.data_, src.size_, DEEP_COPY);
    return *this;
}

inline void PDF::swap(PDF& other) {
    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(alloc_, other.alloc_);
}

inline const char* PDF::data() const {
    return data_;
}

inline bool PDF::isNull() const {
    return ! data_;
}

inline size_t PDF::size() const {
    return size_;
}

inline void PDF::writeTextShort(std::ostream& o) const {
    o << "PDF packet (" << size_ << (size_ == 1 ? " byte)" : " bytes)");
}

inline std::shared_ptr<Packet> PDF::internalClonePacket() const {
    return std::make_shared<PDF>(data_, size_, DEEP_COPY);
}

inline void swap(PDF& a, PDF& b) {
    a.swap(b);
}

} // namespace regina

#endif

