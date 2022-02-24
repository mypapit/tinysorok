/*
This software is under PPSplit Public License 1.1
Copyright (c) 2000-2004 Mypapit. All rights reserved.


This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from 
the use of this software.

Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim 
that you wrote the original software. If you use this software in a product, 
an acknowledgment in the product documentation would be appreciated but is not 
required.

2. Altered source versions must be plainly marked as such, and must not be 
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.

4. Redistributions of source code must with or without modifications retain the 
above copyright notice, this list of conditions and the following disclaimer.

5. Redistributions in binary form with or without modifications must reproduce the 
above copyright notice, this list of conditions and the following disclaimer in the 
documentation and other materials provided with the distribution. 


THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

For latest version of this software, please visit :

http://ppsplit.sourceforge.net
http://papit.cjb.net

authors email : papit58@yahoo.com, papit01@lycos.com

*/

typedef struct{
    int size;
    int reserve1;
    int offset;
    int hdrsize;
    int width;
    int height;
    short planes;
    short bpp;
    int compression;
    int datasize;
    int hres;
    int vres;
    int color;
    int icolor;
}PAPITBMP;
