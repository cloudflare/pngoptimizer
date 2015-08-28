///////////////////////////////////////////////////////////////////////////////
// This file is part of the chustd library
// Copyright (C) ChuTeam
// For conditions of distribution and use, see copyright notice in chustd.h
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DynamicMemoryFile.h"

///////////////////////////////////////////////////////////////////////////////
using namespace chustd;
///////////////////////////////////////////////////////////////////////////////

DynamicMemoryFile::DynamicMemoryFile()
{
	m_byteOrder = boBigEndian;
	m_position = 0;
}

bool DynamicMemoryFile::SetPosition(int64 offset, Whence whence)
{
	int64 newPos = 0;
	if( whence == IFile::posBegin )
	{
		newPos = offset;
	}
	else if( whence == IFile::posEnd )
	{
		newPos = m_position - offset;
	}
	else if( whence == IFile::posCurrent )
	{
		newPos = m_position + offset;
	}

	if( newPos < 0 )
	{
		return false;
	}
	if( newPos > MAX_INT32 )
	{
		return false;
	}

	if( newPos > m_content.GetSize() )
	{
		return false;
	}
	m_position = int32(newPos);
	return true;
}

int64 DynamicMemoryFile::GetPosition() const
{
	return m_position;
}

int64 DynamicMemoryFile::GetSize()
{
	return m_content.GetSize();
}

int DynamicMemoryFile::Read(void* pBuffer, int size)
{
	int remaining = m_content.GetSize() - m_position;
	if( size > remaining )
	{
		size = remaining;
	}

	if( size > 0 )
	{
		const uint8* pContent = m_content.GetReadPtr();
		Memory::Copy(pBuffer, pContent + m_position, size);
		m_position += size;
	}
	return size;
}

int DynamicMemoryFile::Write(const void* pBuffer, int size)
{
	int newSize = m_position + size;
	if( newSize > m_content.GetSize() )
	{
		if( !m_content.SetSize(newSize) )
		{
			return -1;
		}
	}

	uint8* pDst = m_content.GetWritePtr() + m_position;
	Memory::Copy(pDst, pBuffer, size);
	m_position += size;
	return size;
}

ByteOrder DynamicMemoryFile::GetByteOrder() const
{
	return m_byteOrder;
}

void DynamicMemoryFile::SetByteOrder(ByteOrder byteOrder)
{
	m_byteOrder = byteOrder;
}

void DynamicMemoryFile::Close()
{
	m_content.Clear();
	m_byteOrder = boBigEndian;
	m_position = 0;
}

bool DynamicMemoryFile::Open(int32 initialCapacity)
{
	if( initialCapacity < 0 )
	{
		return false;
	}
	m_position = 0;
	return m_content.EnsureCapacity(initialCapacity);
}

bool DynamicMemoryFile::EnsureCapacity(int32 capacity)
{
	return m_content.EnsureCapacity(capacity);
}

const Buffer& DynamicMemoryFile::GetContent()
{
	return m_content;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Writes to this DynamicMemoryFile using another file as the source
int DynamicMemoryFile::WriteFromFile(IFile& fileSrc, int size)
{
	if( size < 0 )
	{
		return -1;
	}
	bool resized = false;
	int newSize = m_position + size;
	if( newSize > m_content.GetSize() )
	{
		if( !m_content.SetSize(newSize) )
		{
			return -1;
		}
		resized = true;
	}

	uint8* pDst = m_content.GetWritePtr() + m_position;
	int32 read = fileSrc.Read(pDst, size);
	m_position += read;
	// Update size in case we read less
	if( resized )
	{
		m_content.SetSize(m_position);
	}
	return read;
}
