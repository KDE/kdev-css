/*****************************************************************************
 * Copyright (c) 2007 Piyush verma <piyush.verma@gmail.com>                  *
 * Copyright (c) 2009, 2010 Niko Sams <niko.sams@gmail.com>                  *
 * Copyright (c) 2010 Milian Wolff <mail@milianw.de>                         *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#ifndef CSS_PARSEJOB_H
#define CSS_PARSEJOB_H

#include <language/backgroundparser/parsejob.h>

#include <QStringList>

#include <ktexteditor/range.h>

namespace Css
{
struct AstNode;
class LanguageSupport;
class EditorIntegrator;

class ParseJob : public KDevelop::ParseJob
{
    Q_OBJECT

public:
    explicit ParseJob(const KDevelop::IndexedString &url, KDevelop::ILanguageSupport* languageSupport);
    ~ParseJob() override;

protected:
    void run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread) override;
};

}

#endif
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
