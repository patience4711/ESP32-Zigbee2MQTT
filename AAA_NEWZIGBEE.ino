
char * readFilteredFrame(char frameHex[], uint8_t whatOps)
{
      // this function extracts zigbee frames from the incoming zigbee
      // and returns a frame depending on whatOps
      char s_d[1024]= {0};
      char comp[5] = {0};
      //s_d[0]='/0';
      char messageToDecode[128] = {0};
      
      if (whatOps == 5)
      {
        consoleOut("we handling a test join char");
        strcpy (messageToDecode, "FE0345C4CB06004FFE20448100000207CB060101007E0089D85E00000C18140A000025000000000000CB061DA3FE0C45CA87E915DB8E6C7838C1A40000E4");
      } else {
      strcpy(messageToDecode, readZB(s_d));   // fills hex string
      }
      size_t pos = 0;
      bool success=false;

      while (extractNextFrameHex(messageToDecode, pos, frameHex))
      {
            consoleOut("extracted frame: " + String(frameHex));
            //Serial.println(frameHex);
            memcpy(comp, frameHex + 4, 4);
            comp[4] = '\0';
            Serial.println("the comp value in frameHex is " + String(comp));
            
            if (whatOps==5 && strcmp(comp, "45CA") == 0)
            {
              consoleOut("→ found join frame");
              success=true;
              break;
            }
            if (whatOps==7 && strcmp(comp, "6700") == 0 )
            {
              consoleOut("→ found healthcheck response");
              success=true;
              break;
            }
            if (whatOps == 1 && strcmp(comp, "4481") == 0 )
            {
              consoleOut("→ found normal response");
              success=true;
              break;
            }
      
      }
      empty_serial2(); // waste the excess bytes
      if(! success) strcpy(frameHex, "fail");
      return frameHex;
}

// char * readFilteredFrame(char frameHex[], uint8_t whatOps)
// {
//       // this function extracts zigbee frames from the incoming zigbee
//       // and returns a frame depending on whatOps
//       char s_d[1024]= {0};
//       //s_d[0]='/0';
//       char messageToDecode[128] = {0};
//       strcpy(messageToDecode, readZB(s_d));   // fills hex string

//       size_t pos = 0;
//       bool success=false;

//       while (extractNextFrameHex(messageToDecode, pos, frameHex))
//       {
//             consoleOut("extracted frame: " + String(frameHex));
//             //Serial.println(frameHex);
            
//             if (whatOps==5 && frameContains(frameHex, "45CA"))
//             {
//               consoleOut("→ found join frame");
//               success=true;
//               break;
//             }
//             if (whatOps==7 && frameContains(frameHex, "6700"))
//             {
//               consoleOut("→ found healthcheck response");
//               success=true;
//               break;
//             }
//             if (whatOps == 1 && frameContains(frameHex, "4481"))
//             {
//               consoleOut("→ found normal response");
//               success=true;
//               break;
//             }
      
//       }
//       empty_serial2(); // waste the excess bytes
//       if(! success) strcpy(frameHex, "fail");
//       return frameHex;
// }


bool extractNextFrameHex(const char *bigHex,
                         size_t &pos,
                         char *tempFrame)
{
 // this function copies the length of the message from pos+2
 // and reads the smount of bytes, then start on the next FE 
  size_t totalLen = strlen(bigHex);

  while (pos + 4 <= totalLen)
  {
    // Look for "FE"
    if (bigHex[pos] != 'F' || bigHex[pos + 1] != 'E')
    {
      pos += 2;
      continue;
    }
    //
    // now we have FE we Read LEN byte
    uint8_t len = hexByte(&bigHex[pos + 2]);

    // Total frame size in hex characters
    size_t frameChars = (len + 3) * 2;

    // Bounds check
    if (pos + frameChars > totalLen)
      return false;

    // Copy frame into temp buffer
    memcpy(tempFrame, &bigHex[pos], frameChars);
    tempFrame[frameChars] = '\0';

    // Advance position
    pos += frameChars;

    return true;
  }

  return false;
}

// bool frameContains(const char *frameHex, const char *pattern)
// {
//   return strstr(frameHex, pattern) != nullptr;
// }

uint8_t hexByte(const char *p)
{
  uint8_t v = 0;
  for (int i = 0; i < 2; i++)
  {
    char c = p[i];
    v <<= 4;
    if (c >= '0' && c <= '9') v |= (c - '0');
    else if (c >= 'A' && c <= 'F') v |= (c - 'A' + 10);
    else if (c >= 'a' && c <= 'f') v |= (c - 'a' + 10);
  }
  return v;
}



