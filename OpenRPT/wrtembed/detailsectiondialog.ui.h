/*
 * Copyright (c) 2002-2005 by OpenMFG, LLC
 *
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * If you do not wish to be bound by the terms of the GNU General Public
 * License, DO NOT USE THIS SOFTWARE.  Please contact OpenMFG, LLC at
 * info@openmfg.com for details on how to purchase a commercial license.
 */

#include "reportsection.h"
#include "detailgroupsectiondialog.h"
#include <qmessagebox.h>
#include <qcheckbox.h>

void DetailSectionDialog::btnAdd_clicked()
{
    if(_rsd) {
	    // lets add a new section
	    QString name = "unnamed";
	    int i = 0;
	    while(i < 100 && _rsd->findSection(name) != -1) {
	        i++;
	        name = QString().sprintf("unnamed%d", i);
	    }
	    if(_rsd->findSection(name) != -1) {
            QMessageBox::warning(this, tr("Error Encountered"), tr("Unable to add a new group because a non-unique name could be generated."));
	        return;
	    }
	    ReportSectionDetailGroup * rsdg = new ReportSectionDetailGroup(name, _rsd, _rsd);
	    _rsd->insertSection(_rsd->groupSectionCount(), rsdg);
	    lbGroups->insertItem(name);
	    lbGroups->setCurrentItem(lbGroups->count()-1);
	    btnEdit_clicked();
    }
}


void DetailSectionDialog::btnEdit_clicked()
{
    if(_rsd) {
	    int idx = lbGroups->currentItem();
	    if(idx < 0) return;
        ReportSectionDetailGroup * rsdg = _rsd->getSection(idx);
        DetailGroupSectionDialog * dgsd = new DetailGroupSectionDialog(this);
        dgsd->tbName->setText(rsdg->getTitle());
        dgsd->tbColumn->setText(rsdg->column());
        dgsd->cbHead->setChecked(rsdg->isGroupHeadShowing());
        dgsd->cbFoot->setChecked(rsdg->isGroupFootShowing());

        bool exitLoop = FALSE;
        while(!exitLoop) {
            if(dgsd->exec() == QDialog::Accepted) {
                QString name = dgsd->tbName->text();
                QString column = dgsd->tbColumn->text();
                bool showgh = dgsd->cbHead->isChecked();
                bool showgf = dgsd->cbFoot->isChecked();

                if(name != rsdg->getTitle() && _rsd->findSection(name) != -1) {
                    QMessageBox::warning(this, tr("Error Encountered"),
                        tr("Tried to add a new Group section with a non-unique name."));
                }
                else {
                    if(name != rsdg->getTitle()) {
                        rsdg->setTitle(name);
                        lbGroups->changeItem(name,idx);
                    }
                    rsdg->setColumn(column);
                    rsdg->showGroupHead(showgh);
                    rsdg->showGroupFoot(showgf);

                    exitLoop = TRUE;
                }
            } else {
                exitLoop = TRUE;
            }
        }

        if(dgsd) delete dgsd;
    }
}


void DetailSectionDialog::btnRemove_clicked()
{
    if(_rsd) {
	    int idx = lbGroups->currentItem();
	    if(idx != -1) {
	        lbGroups->removeItem(idx);
	        _rsd->removeSection(idx, TRUE);
	    }
    }
}


void DetailSectionDialog::btnMoveUp_clicked()
{
    if(_rsd) {
	    int idx = lbGroups->currentItem();
	    if(idx <= 0) return;
	    QString s = lbGroups->currentText();
	    lbGroups->removeItem(idx);
	    lbGroups->insertItem(s, idx-1);
	    ReportSectionDetailGroup * rsdg = _rsd->getSection(idx);
        bool showgh = rsdg->isGroupHeadShowing();
        bool showgf = rsdg->isGroupFootShowing();
	    _rsd->removeSection(idx);
	    _rsd->insertSection(idx-1,rsdg);
        rsdg->showGroupHead(showgh);
        rsdg->showGroupFoot(showgf);
    }
}


void DetailSectionDialog::brnMoveDown_clicked()
{
    if(_rsd) {
	    int idx = lbGroups->currentItem();
	    if(idx == (int)(lbGroups->count() - 1)) return;
	    QString s = lbGroups->currentText();
	    lbGroups->removeItem(idx);
	    lbGroups->insertItem(s, idx+1);
	    ReportSectionDetailGroup * rsdg = _rsd->getSection(idx);
        bool showgh = rsdg->isGroupHeadShowing();
        bool showgf = rsdg->isGroupFootShowing();
	    _rsd->removeSection(idx);
	    _rsd->insertSection(idx+1, rsdg);
        rsdg->showGroupHead(showgh);
        rsdg->showGroupFoot(showgf);
    }
}


void DetailSectionDialog::init()
{
    _rsd = 0;
    lbGroups->clear();
}


void DetailSectionDialog::setReportSectionDetail( ReportSectionDetail * rsd)
{
    _rsd = rsd;
    lbGroups->clear();
    if(_rsd) {
        for(int i = 0; i < _rsd->groupSectionCount(); i++) {
            lbGroups->insertItem(_rsd->getSection(i)->getTitle());
        }
    }
}