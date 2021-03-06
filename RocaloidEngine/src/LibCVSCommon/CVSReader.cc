 /*
  * CVSReader.cc
  *
  * Copyright (C) 2013 - Rocaloid Development Group(RDG)
  *
  * Created by Sleepwalking
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program. If not, see <http://www.gnu.org/licenses/>.
  */
#include "LibCyberBase/SPKit/defs.h"
#include "LibCyberBase/SPKit/io/terminal.h"
#include "LibCyberBase/SPKit/structure/string.h"
#include "LibCyberBase/SPKit/misc/converter.h"
#include "LibCyberBase/SPKit/structure/array.h"
#include "LibCyberBase/SPKit/io/fileStream.h"

#include "LibCyberBase/Overall.h"
#include "CVSCommon.h"
#include "CVSReader.h"

using namespace RDLIO;
using namespace converter;
namespace CVSCommon
{
	void CVSReader::Open(string FileName)
	{
		Reader.Open(FileName);
		if(Reader.Read() != "#CVS")
			Exception("The file is not a Cybervoice Script!");
		if(Reader.Read() != "1.6")
			Exception("Wrong CVS version.");
	}
	void CVSReader::Close()
	{
		Reader.Close();
	}

	void CVSReader::Read(CVS& _CVS)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "SegmentListQ")
			{
				_CVS.SegmentListQ = TestIfIsIntAndPositive(Reader.Read()) - 1;
				_CVS.SegmentList.setUbound(_CVS.SegmentListQ);
			}else if(StrBuff == "SegmentList")
			{
				int i;
				for(i = 0;i <= _CVS.SegmentListQ;i ++)
				{
					Segment_Read(_CVS.SegmentList[i]);
				}
				if(Reader.Read() != "End")
					Exception("List without and end.");
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::Segment_Read(Segment& _Segment)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "TPhoneListQ")
			{
				int i;
				_Segment.TPhoneListQ = TestIfIsIntAndPositive(Reader.Read()) - 1;
				_Segment.TPhoneList.setUbound(_Segment.TPhoneListQ);
				
				_Segment.Effects.OpennessList.setUbound(_Segment.TPhoneListQ + 1);
				for(i = 0;i <= _Segment.TPhoneListQ + 1;i ++)
					_Segment.Effects.OpennessList[i] = 1;
			}else if(StrBuff == "TPhoneList")
			{
				int i;
				for(i = 0;i <= _Segment.TPhoneListQ;i ++)
				{
					TPhone_Read(_Segment.TPhoneList[i]);
				}
				if(Reader.Read() != "End")
					Exception("List without and end.");
			}else if(StrBuff == "FreqListQ")
			{
				_Segment.FreqListQ = TestIfIsIntAndPositive(Reader.Read()) - 1;
				_Segment.FreqList.setUbound(_Segment.FreqListQ);
			}else if(StrBuff == "FreqList")
			{
				int i;
				for(i = 0;i <= _Segment.FreqListQ;i ++)
				{
					FreqSet_Read(_Segment.FreqList[i]);
				}
				if(Reader.Read() != "End")
					Exception("List without and end.");
			}else if(StrBuff == "Effects")
			{
				Effects_Read(_Segment.Effects);
			}else if(StrBuff == "StartTime")
			{
				_Segment.StartTime = TestIfIsDoubleNotNegative(Reader.Read());
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::TPhone_Read(TPhone& _TPhone)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Start")
			{
				TStart_Read(_TPhone.Start);
			}else if(StrBuff == "Dest")
			{
				TDest_Read(_TPhone.Dest);				
			}else if(StrBuff == "Transition")
			{
				TTransition_Read(_TPhone.Transition);				
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::TStart_Read(TStart& _TStart)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Type")
			{
				StrBuff = Reader.Read();
				StrBuff = lowerCase(StrBuff);
				if(StrBuff == "new")
					_TStart.Type = true;
				else if(StrBuff == "preserved")
					_TStart.Type = false;
				else
					Exception(CStr("Invalid TPhone type identifier as ") + StrBuff + CStr("."));
			}else if(StrBuff == "Symbol")
			{
				_TStart.Symbol = Reader.Read();
			}else if(StrBuff == "Preserved")
			{
				_TStart.Preserved = TestIfIsIntAndPositive(Reader.Read());
				if(_TStart.Preserved != 1 && _TStart.Preserved != 2)
					Exception(CStr(_TStart.Preserved) + CStr("is not a valid preserved buffer."));
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::TDest_Read(TDest& _TDest)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Type")
			{
				StrBuff = Reader.Read();
				StrBuff = lowerCase(StrBuff);
				if(StrBuff == "new")
					_TDest.Type = true;
				else if(StrBuff == "preserved")
					_TDest.Type = false;
				else
					Exception(CStr("Invalid TPhone type identifier as ") + StrBuff + CStr("."));
			}else if(StrBuff == "Symbol")
			{
				_TDest.Symbol = Reader.Read();
			}else if(StrBuff == "Preserved")
			{
				_TDest.Preserved = TestIfIsIntAndPositive(Reader.Read());
				if(_TDest.Preserved != 1 && _TDest.Preserved != 2)
					Exception(CStr(_TDest.Preserved) + CStr("is not a valid preserved buffer."));
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::TTransition_Read(TTransition& _TTransition)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "StartRatio")
			{
				_TTransition.StartRatio = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "EndRatio")
			{
				_TTransition.EndRatio = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "Time")
			{
				_TTransition.Time = TestIfIsDoubleAndPositive(Reader.Read());
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::FreqSet_Read(FreqSet& _FreqSet)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Time")
			{
				_FreqSet.Time = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "Freq")
			{
				_FreqSet.Freq = TestIfIsDoubleAndPositive(Reader.Read());
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::Effects_Read(EffectCollection& _Effects)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Shrink")
			{
				_Effects.Shrink = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "ForwardCut")
			{
				_Effects.ForwardCut = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "EnvelopeListQ")
			{
				_Effects.EnvelopeListQ = TestIfIsIntAndPositive(Reader.Read()) - 1;
				_Effects.EnvelopeList.setUbound(_Effects.EnvelopeListQ + 1);
			}else if(StrBuff == "EnvelopeList")
			{
				int i;
				for(i = 0;i <= _Effects.EnvelopeListQ;i ++)
				{
					EnvelopeSet_Read(_Effects.EnvelopeList[i]);
				}
				if(Reader.Read() != "End")
					Exception("List without and end.");
			}else if(StrBuff == "ElistEnabled")
			{
				_Effects.ElistEnabled = TestIfIsBoolean(Reader.Read());
			}else if(StrBuff == "PElopeEnabled")
			{
				_Effects.PElopeEnabled = TestIfIsBoolean(Reader.Read());
			}else if(StrBuff == "PresetedEnvelope")
			{
				_Effects.PresetedEnvelope = TestIfIsPresetedEnvelope(Reader.Read());
			}else if(StrBuff == "ADSR")
			{
				ADSREnvelope_Read(_Effects.ADSR);
			}else if(StrBuff == "OpennessList")
			{
				int i;
				int ubound = _Effects.OpennessList.getUbound();
				for(i = 0;i <= ubound;i ++)
					_Effects.OpennessList[i] = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "Vibration")
			{
				//Preserved.
			}else if(StrBuff == "Breath")
			{
				BreathStruct_Read(_Effects.Breath);
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::EnvelopeSet_Read(EnvelopeSet& _EnvelopeSet)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Time")
			{
				_EnvelopeSet.Time = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "Amplitude")
			{
				_EnvelopeSet.Amplitude = TestIfIsDoubleNotNegative(Reader.Read());
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::ADSREnvelope_Read(ADSREnvelope& _ADSREnvelope)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Amplitude")
			{
				_ADSREnvelope.Amplitude = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "Attack")
			{
				_ADSREnvelope.Attack = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "Decline")
			{
				_ADSREnvelope.Decline = TestIfIsDoubleNotNegative(Reader.Read());
			}else if(StrBuff == "Release")
			{
				_ADSREnvelope.Release = TestIfIsDoubleNotNegative(Reader.Read());
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
	void CVSReader::BreathStruct_Read(BreathStruct& _Breath)
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "Magnitude")
			{
				_Breath.Magnitude = TestIfIsDouble(Reader.Read());
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
};

/* template
	{
		string StrBuff;
		do
		{
			StrBuff = Reader.Read();
			if(StrBuff == "SegmentListQ")
			{
				StrBuff = Reader.Read();
				_CVS.SegmentListQ = TestIfIsIntAndPositive(StrBuff) - 1;
				_CVS.SegmentList.setUbound(_CVS.SegmentListQ);
			}else if(StrBuff == "SegmentList")
			{
				int i;
				for(i = 0;i <= _CVS.SegmentListQ;i ++)
				{
					//Segment_Read(_CVS.SegmentList[i]);
				}
				if(Reader.Read() != "End")
					Exception("List without and end.");
			}else
			{
				//Exception
			}
		}while(StrBuff != "End");
	}
*/