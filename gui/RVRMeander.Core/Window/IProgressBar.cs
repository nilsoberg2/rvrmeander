﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Progress
{
  public interface IProgressBar
  {
    void SetProgress(int progress);

    void Reset();
  }
}