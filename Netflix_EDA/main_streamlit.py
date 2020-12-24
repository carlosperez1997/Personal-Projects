import pandas as pd 
import numpy as np 
import matplotlib.pyplot as plt 
import seaborn as sns

from datetime import date
from datetime import datetime

import streamlit as st
import os

st.write("""
    # Hello
    """)

#streamlit run main_streamlit.py

#File > Preferences > Settings > Workspace > Extensions > Python

def clean_and_prepare_data (netflix_vh):    
    # Drop NaN titles
    netflix_hist = netflix_vh.dropna()

    assert netflix_hist['Title'].isna().sum() == 0

    netflix_hist.set_index('Title', inplace=True)

    netflix_hist.loc["is_TV_show"] = False

    netflix_hist['Date'] = pd.to_datetime( netflix_hist['Date'] )

    first_day = min(netflix_hist['Date'])
    last_day = max(netflix_hist['Date'])
    
    for lab, row  in netflix_hist.iterrows():
        title = str(lab)
        is_TVshow = ['Temporada' in title, 'Season' in title, 'Serie' in title, 'Miniserie' in title, 'Capítulo' in title, 'Episode' in title, 'Parte' in title]    
        netflix_hist.loc[lab, "is_TV_show"] = max(is_TVshow)
    
    return(netflix_hist) 