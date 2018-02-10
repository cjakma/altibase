/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 

/***********************************************************************
 * $Id: qc.h 82186 2018-02-05 05:17:56Z lswhh $
 **********************************************************************/

#ifndef _O_QC_H_
#define _O_QC_H_ 1

#include <iduMemory.h>
#include <iduMemListOld.h>
#include <iduMutex.h>
#include <iduFixedTable.h>
#include <iduVarMemList.h>
#include <idv.h>
#include <mtcDef.h>
#include <qcuError.h>
#include <qmcCursor.h>
#include <qmcTempTableMgr.h>
#include <qmxSessionCache.h>
#include <mtlTerritory.h>
#include <mtuProperty.h>
#include <qciStmtType.h>

struct qtcNode;
struct qtcCacheObj;
struct sdiClientInfo;

#if defined(COMPILE_FOR_PAGE64)
#define QCM_DEF_ITER(A)  ULong A[4096]
#else
#define QCM_DEF_ITER(A)  ULong A[2048]
#endif


// for TIMESTAMP
#define QC_BYTE_PRECISION_FOR_TIMESTAMP               (8)

// for mm
#define QC_MAX_NAME_LEN                               (40)
#define QC_MAX_OBJECT_NAME_LEN                        (128) //BUG-39579

/* _NEXT_MSG_ID */
#define QC_MAX_SEQ_NAME_LEN                          (QC_MAX_OBJECT_NAME_LEN + 13)

#define QC_MAX_COLUMN_COUNT       (SMI_COLUMN_ID_MAXIMUM)
#define QC_MAX_KEY_COLUMN_COUNT     (SMI_MAX_IDX_COLUMNS)
#define QC_MAX_STATEMENT                           (1024)
#define QC_TEMPLATE_MAX_STACK_COUNT               (65536)

// PROJ-1502 PARTITIONED DISK TABLE
#define QC_MAX_PARTKEY_COND_COUNT                  (2000)
#define QC_MAX_PARTKEY_COND_VALUE_LEN              (4000)

// PROJ-1638 Selection Replication
#define QC_CONDITION_LEN                           (1000)

// BUG-21122
#define QC_AUTO_REMOTE_EXEC_ENABLE                 (1)
#define QC_AUTO_REMOTE_EXEC_DISABLE                (0)

// BUG-21387 COMMENT
#define QC_MAX_COMMENT_LITERAL_LEN                 (4000)
#define QC_MAX_COMMENT_LITERAL_LEN_STR             "4000"

// PROJ-2002 Column Security
#define QC_MAX_IP_LEN                              (64)

/* PROJ-2207 Password policy support */
#define QC_ACCOUNT_STATUS_LEN                      (40)
#define QC_PASSWORD_OPT_LEN                        (40)
#define QC_PASSWORD_OPT_MAX_COUNT_VALUE            (1000)
#define QC_PASSWORD_OPT_MAX_DATE                   (36500)
#define QC_PASSWORD_OPT_USER_ERROR_LEN             (1024)
#define QC_PASSWORD_CURRENT_DATE_LEN               (1024)

/* PROJ-1090 Function-based Index */
/* ~$IDX32 */
#define QC_MAX_FUNCTION_BASED_INDEX_NAME_LEN       (QC_MAX_OBJECT_NAME_LEN - 6)

/* BUG-11561
 * �Լ��� ���ú����� Char sBuffer [ QC_MAX_OBJECT_NAME_LEN + 2 ] �� ���۸� ��Ƽ�
 * �� ������ �ٷ� mtdCharType * ���� Ÿ�� ĳ���� �Ͽ� ����� ��
 * sBuffer �� align���� �ʾƼ� ���������� �߻��մϴ�.
 *
 * �׷��� QC_MAX_OBJECT_NAME_LEN��ŭ�� ���ڿ��� ���� �� �� �ִ� mtdCharType ���۸�
 * ���� ���ÿ� ����� �� ������ �Ʒ� qcNameCharBuffer�� �����Ͽ���
 * mtdCharType * �� ĳ������ ���ÿ� ���� ������ �߻����� �ʽ��ϴ�.
 *
 * by kmkim
 */
typedef struct qcNameCharBuffer {
    SDouble buffer[ (ID_SIZEOF(UShort) + QC_MAX_OBJECT_NAME_LEN + 7) / 8 ];
} qcNameCharBuffer;

typedef struct qcSeqNameCharBuffer {
    SDouble buffer[ (ID_SIZEOF(UShort) + QC_MAX_SEQ_NAME_LEN + 7) / 8 ];
} qcSeqNameCharBuffer;

typedef struct qcCondValueCharBuffer {
    SDouble buffer[ (ID_SIZEOF(UShort) + QC_MAX_PARTKEY_COND_VALUE_LEN + 7) / 8 ];
} qcCondValueCharBuffer;


// for qcNamePosition
#define QC_POS_EMPTY_OFFSET                          (-1)
#define QC_POS_EMPTY_SIZE                            (-1)

// for ORDER BY
#define     QMV_EMPTY_TARGET_POSITION                 (0)

/* mtcTuple->row size of constant row */
#define QC_CONSTANT_FIRST_ROW_SIZE                 (4096)
#define QC_CONSTANT_ROW_SIZE                      (65536)

// for referential action
#define QC_REF_ACT_MASK                      (0x000000FF)
#define QC_REF_ACT_DEL_MASK                  (0x0000000F)
#define QC_REF_ACT_UPD_MASK                  (0x000000F0)
#define QC_REF_ACT_DEL_NOACT                 (0x00000001)
#define QC_REF_ACT_DEL_RESTR                 (0x00000002)
#define QC_REF_ACT_UPD_NOACT                 (0x00000010)
#define QC_REF_ACT_UPD_RESTR                 (0x00000020)

// for qcmPrivilege
#define QC_PRIV_MASK                         (0x000000FF)
#define QC_PRIV_SEL                          (0x00000001)
#define QC_PRIV_INS                          (0x00000002)
#define QC_PRIV_UPD                          (0x00000004)
#define QC_PRIV_DEL                          (0x00000008)
#define QC_PRIV_DDL                          (0x00000010)

// for qcTemplate.flag
#define QC_TMP_INITIALIZE                    (0x00000000)

// for qcTemplate.flag
#define QC_TMP_COLUMN_BIND_MASK              (0x00000001)
#define QC_TMP_COLUMN_BIND_FALSE             (0x00000000)
#define QC_TMP_COLUMN_BIND_TRUE              (0x00000001)

// BUG-16422
// for qcTemplate.flag
#define QC_TMP_EXECUTION_MASK                (0x00000002)
#define QC_TMP_EXECUTION_FAILURE             (0x00000000)
#define QC_TMP_EXECUTION_SUCCESS             (0x00000002)

// PROJ-1436
// for qcTemplate.flag
// plan cache ��������� cache�� �� ���� DML�� ���Ͽ�
// plan cache �� �� ������ ǥ���Ѵ�.
// ��) select * from D$TBS_FREE_EXTLIST(...);
//     select * from t1@link1;
//     select /* no_plan_cache */ * from t1;
#define QC_TMP_PLAN_CACHE_IN_MASK            (0x00000008)
#define QC_TMP_PLAN_CACHE_IN_ON              (0x00000000)
#define QC_TMP_PLAN_CACHE_IN_OFF             (0x00000008)

// PROJ-1436
// for qcTemplate.flag
// ������ plan�� ���Ͽ� ������� ���濡�� ������ plan��
// �״�� ����� �� �ְ� �Ѵ�. ������ cache�� plan��
// ���Ͽ� ������� ���濡�� plan�� ��� ������ ��������
// ����Ǿ�����, cache���� ���� plan�̴��� prepare-
// execute�� ����Ǵ� ������ ���Ͽ� ������� ���濡��
// plan�� �״�� ����� �� �ֵ��� �Ѵ�.
// ��) select /* no_plan_cache keep_plan */ * from t1;
#define QC_TMP_PLAN_KEEP_MASK                (0x00000010)
#define QC_TMP_PLAN_KEEP_FALSE               (0x00000000)
#define QC_TMP_PLAN_KEEP_TRUE                (0x00000010)

// BUG-35037
// CREATE VIEW AS SELECT ������ ����� �� WIHT�� ����
// RELATED OBJECT�� ��� ���� VIEW�� RELATED OBJECT�� ���� �ϰ� �ȴ�.
// FLAG�� ���� �ϸ� parseSelect()�Լ��� ���� parseViewInFromClause()
// �Լ����� parseSelect()�� ��� ȣ�� ���� �ʰ� �ǰ� REALTED OBJECT��
// ���Ե��� �ʴ´�.
#define QC_PARSE_CREATE_VIEW_MASK            (0x00000020)
#define QC_PARSE_CREATE_VIEW_FALSE           (0x00000000)
#define QC_PARSE_CREATE_VIEW_TRUE            (0x00000020)

// BUG-38508
#define QC_TMP_REF_FIXED_TABLE_MASK          (0x00000040)
#define QC_TMP_REF_FIXED_TABLE_FALSE         (0x00000000)
#define QC_TMP_REF_FIXED_TABLE_TRUE          (0x00000040)

/*
 * BUG-39441 replication table ��������
 * qcTemplate.flag
 */
#define QC_TMP_REF_REPL_TABLE_MASK           (0x00000080)
#define QC_TMP_REF_REPL_TABLE_FALSE          (0x00000000)
#define QC_TMP_REF_REPL_TABLE_TRUE           (0x00000080)

// BUG-41615 simple query hint
#define QC_TMP_EXEC_FAST_MASK                (0x00000300)
#define QC_TMP_EXEC_FAST_NONE                (0x00000000)
#define QC_TMP_EXEC_FAST_TRUE                (0x00000100)
#define QC_TMP_EXEC_FAST_FALSE               (0x00000200)

/* BUG-42639 Monitoring query
 * Queyr�� ��� X$�� V$ Fixed Table�� �̷���� �ִ��� üũ
 * X$ �߿����� Traction�� ����ϴ� ���� �����ϰ�
 * Query�� Function�̳� Sequence�� ���Ǵ� �͵� �����Ѵ�.
 * �� user�� ���� ������ created view �� �Ϲ� ���̺��� ���Ե�
 * ��쿡�� �����Ѵ�.
 * �� flag�� TRUE �� �Ǹ� MM���� Transction�� �Ҵ����� �ʰԵȴ�.
 */
#define QC_TMP_ALL_FIXED_TABLE_MASK          (0x00000400)
#define QC_TMP_ALL_FIXED_TABLE_FALSE         (0x00000400)
#define QC_TMP_ALL_FIXED_TABLE_TRUE          (0x00000000)

// shard transform ���� ����
#define QC_TMP_SHARD_TRANSFORM_MASK          (0x00000800)
#define QC_TMP_SHARD_TRANSFORM_DISABLE       (0x00000800)
#define QC_TMP_SHARD_TRANSFORM_ENABLE        (0x00000000)

// PROJ-2551 simple query ����ȭ
// simple query�ΰ�
// qcStatement.mFlag
#define QC_STMT_FAST_EXEC_MASK               (0x00000001)
#define QC_STMT_FAST_EXEC_FALSE              (0x00000000)
#define QC_STMT_FAST_EXEC_TRUE               (0x00000001)

// simple query�� simple bind�ΰ�
// qcStatement.mFlag
#define QC_STMT_FAST_BIND_MASK               (0x00000002)
#define QC_STMT_FAST_BIND_FALSE              (0x00000000)
#define QC_STMT_FAST_BIND_TRUE               (0x00000002)

// simple query�� ù��° result�ΰ�
// qcStatement.mFlag
#define QC_STMT_FAST_FIRST_RESULT_MASK       (0x00000004)
#define QC_STMT_FAST_FIRST_RESULT_FALSE      (0x00000000)
#define QC_STMT_FAST_FIRST_RESULT_TRUE       (0x00000004)

// simple query�� ����� ���� �����߳�
// qcStatement.mFlag
#define QC_STMT_FAST_COPY_RESULT_MASK        (0x00000008)
#define QC_STMT_FAST_COPY_RESULT_FALSE       (0x00000000)
#define QC_STMT_FAST_COPY_RESULT_TRUE        (0x00000008)

// BUG-45443
// qcStatement.mFlag
#define QC_STMT_SHARD_OBJ_MASK               (0x00000010)
#define QC_STMT_SHARD_OBJ_ABSENT             (0x00000000)
#define QC_STMT_SHARD_OBJ_EXIST              (0x00000010)

// for qcSession.flag
#define QC_SESSION_ALTER_META_MASK           (0x00000001)
#define QC_SESSION_ALTER_META_DISABLE        (0x00000000)
#define QC_SESSION_ALTER_META_ENABLE         (0x00000001)

// for qcSession.flag
// qcg::allocStatement() ����
// ���ڷ� �Ѿ���� session ��ü�� null�� ���,
// ���ο��� session��ü�� �Ҵ�ް�
// session ���� ���ٽ� default ������ �򵵷� �Ѵ�.
#define QC_SESSION_INTERNAL_ALLOC_MASK       (0x00000002)
#define QC_SESSION_INTERNAL_ALLOC_FALSE      (0x00000000)
#define QC_SESSION_INTERNAL_ALLOC_TRUE       (0x00000002)

// BUG-26017
// [PSM] server restart�� ����Ǵ�
// psm load�������� ����������Ƽ�� �������� ���ϴ� ��� ����.
#define QC_SESSION_INTERNAL_LOAD_PROC_MASK   (0x00000004)
#define QC_SESSION_INTERNAL_LOAD_PROC_FALSE  (0x00000000)
#define QC_SESSION_INTERNAL_LOAD_PROC_TRUE   (0x00000004)

#define QC_SESSION_INTERNAL_JOB_MASK         (0x00000008)
#define QC_SESSION_INTERNAL_JOB_FALSE        (0x00000000)
#define QC_SESSION_INTERNAL_JOB_TRUE         (0x00000008)

// for qcSession.flag
#define QC_SESSION_INTERNAL_EXEC_MASK        (0x00000010)
#define QC_SESSION_INTERNAL_EXEC_FALSE       (0x00000000)
#define QC_SESSION_INTERNAL_EXEC_TRUE        (0x00000010)

// BUG-45385
// for qcSession.flag
// ���� tx���� shard meta�� �����ߴ�. tx end�� �ʱ�ȭ�Ѵ�.
#define QC_SESSION_SHARD_META_TOUCH_MASK     (0x00000020)
#define QC_SESSION_SHARD_META_TOUCH_FALSE    (0x00000000)
#define QC_SESSION_SHARD_META_TOUCH_TRUE     (0x00000020)

// BUG-45385
// for qcSession.flag
// ���� session���� plan cache�� ������� �ʴ´�.
#define QC_SESSION_PLAN_CACHE_MASK           (0x00000040)
#define QC_SESSION_PLAN_CACHE_ENABLE         (0x00000000)
#define QC_SESSION_PLAN_CACHE_DISABLE        (0x00000040)

#define SET_POSITION(_DESTINATION_, _SOURCE_)       \
{                                                   \
    (_DESTINATION_).stmtText = (_SOURCE_).stmtText; \
    (_DESTINATION_).offset   = (_SOURCE_).offset;   \
    (_DESTINATION_).size     = (_SOURCE_).size;     \
}

#define SET_QUOTE_POSITION(_DESTINATION_, _SOURCE_) \
{                                                   \
    (_DESTINATION_).stmtText = (_SOURCE_).stmtText; \
    (_DESTINATION_).offset   = (_SOURCE_).offset+1; \
    (_DESTINATION_).size     = (_SOURCE_).size-2;   \
}

#define SET_EMPTY_POSITION(_DESTINATION_)           \
{                                                   \
    (_DESTINATION_).stmtText = NULL;                \
    (_DESTINATION_).offset   = QC_POS_EMPTY_OFFSET; \
    (_DESTINATION_).size     = QC_POS_EMPTY_SIZE;   \
}

#define STRUCT_ALLOC(_iduMemory_, _structType_, _memPtr_)                               \
    (_iduMemory_)->alloc(idlOS::align8(ID_SIZEOF(_structType_)), (void**)(_memPtr_))

#define STRUCT_ALLOC_WITH_SIZE(_iduMemory_, _structType_, _size_, _memPtr_)     \
    (_iduMemory_)->alloc((_size_), (void**)(_memPtr_))

#define STRUCT_ALLOC_WITH_COUNT(_iduMemory_, _structType_, _count_, _memPtr_)   \
    (_iduMemory_)->alloc(                                                       \
        idlOS::align8(ID_SIZEOF(_structType_) * (_count_)), (void**)(_memPtr_))

#define STRUCT_CRALLOC(_iduMemory_, _structType_, _memPtr_)                             \
    (_iduMemory_)->cralloc(idlOS::align8(ID_SIZEOF(_structType_)), (void**)(_memPtr_))

#define STRUCT_CRALLOC_WITH_SIZE(_iduMemory_, _structType_, _size_, _memPtr_)     \
    (_iduMemory_)->cralloc((_size_), (void**)(_memPtr_))

#define STRUCT_CRALLOC_WITH_COUNT(_iduMemory_, _structType_, _count_, _memPtr_)   \
    (_iduMemory_)->cralloc(                                                       \
        idlOS::align8(ID_SIZEOF(_structType_) * (_count_)), (void**)(_memPtr_))

#define QC_IS_NULL_NAME(_name_)                                 \
    ( (_name_.size <= QC_POS_EMPTY_SIZE) ? ID_TRUE : ID_FALSE )

#define QC_IS_NAME_MATCHED( _position1_, _position2_)                           \
    ( (idlOS::strMatch(                                                         \
           _position1_.stmtText+_position1_.offset, _position1_.size,           \
           _position2_.stmtText+_position2_.offset, _position2_.size) == 0) ?   \
      ID_TRUE :                                                                 \
      ID_FALSE )

#define QC_IS_NAME_MATCHED_OR_EMPTY( _position1_, _position2_ )                                      \
    ( ( ( _position1_.size > QC_POS_EMPTY_SIZE ) && ( _position2_.size > QC_POS_EMPTY_SIZE ) ) ?   \
      ( ( idlOS::strMatch( _position1_.stmtText + _position1_.offset, _position1_.size,              \
                           _position2_.stmtText + _position2_.offset, _position2_.size ) == 0 ) ?    \
        ID_TRUE : ID_FALSE ) :                                                                       \
      ( ( ( _position1_.size <= QC_POS_EMPTY_SIZE ) && ( _position2_.size <= QC_POS_EMPTY_SIZE ) ) ? \
        ID_TRUE : ID_FALSE ) )

#define QC_IS_STR_CASELESS_MATCHED( _position_, _str_ )                         \
    ( (idlOS::strCaselessMatch(_position_.stmtText+_position_.offset,           \
                               _position_.size,                                 \
                               _str_,                                           \
                               idlOS::strlen(_str_)) == 0 ) ? ID_TRUE : ID_FALSE )

#define QC_IS_NAME_CASELESS_MATCHED( _position1_, _position2_)                  \
    ( (idlOS::strCaselessMatch(                                                 \
           _position1_.stmtText+_position1_.offset, _position1_.size,           \
           _position2_.stmtText+_position2_.offset, _position2_.size) == 0) ?   \
      ID_TRUE :                                                                 \
      ID_FALSE )

#define QC_STR_COPY( _charBuf_, _namePos_ )                     \
    (_charBuf_)[ (_namePos_).size ] = '\0';                     \
    idlOS::memcpy( (_charBuf_),                                 \
                   (_namePos_).stmtText + (_namePos_).offset,   \
                   (_namePos_).size );

#define QC_STR_UPPER( _namePos_ )                               \
    idlOS::strUpper((_namePos_).stmtText + (_namePos_).offset,  \
                    (_namePos_).size );

#define QC_PASSWORD_UPPER( _namePos_ )          \
    if ( MTU_CASE_SENSITIVE_PASSWORD == 0 )     \
    {                                           \
        QC_STR_UPPER( _namePos_ );              \
    }

#define QC_SET_STATEMENT(_dst_, _src_, _parse_)                             \
{                                                                           \
    (_dst_)->myPlan                      = &((_dst_)->privatePlan);         \
    (_dst_)->myPlan->planEnv             = (_src_)->myPlan->planEnv;        \
    (_dst_)->myPlan->stmtText            = (_src_)->myPlan->stmtText;       \
    (_dst_)->myPlan->stmtTextLen         = (_src_)->myPlan->stmtTextLen;    \
    (_dst_)->myPlan->mPlanFlag           = (_src_)->myPlan->mPlanFlag;      \
    (_dst_)->myPlan->mHintOffset         = (_src_)->myPlan->mHintOffset;    \
    (_dst_)->myPlan->qmpMem              = (_src_)->myPlan->qmpMem;         \
    (_dst_)->myPlan->qmuMem              = (_src_)->myPlan->qmuMem;         \
    (_dst_)->myPlan->parseTree           = (qcParseTree*)(_parse_);         \
    (_dst_)->myPlan->hints               = NULL;                            \
    (_dst_)->myPlan->plan                = NULL;                            \
    (_dst_)->myPlan->graph               = NULL;                            \
    (_dst_)->myPlan->scanDecisionFactors = NULL;                            \
    (_dst_)->myPlan->procPlanList        = NULL;                            \
    (_dst_)->myPlan->mShardAnalysis      = NULL;                            \
    (_dst_)->myPlan->mShardParamOffset   = ID_USHORT_MAX;                   \
    (_dst_)->myPlan->mShardParamCount    = 0;                               \
    (_dst_)->myPlan->sTmplate            = (_src_)->myPlan->sTmplate;       \
    (_dst_)->myPlan->sBindColumn         = NULL;                            \
    (_dst_)->myPlan->sBindColumnCount    = 0;                               \
    (_dst_)->myPlan->sBindParam          = NULL;                            \
    (_dst_)->myPlan->sBindParamCount     = 0;                               \
    (_dst_)->myPlan->stmtListMgr         = (_src_)->myPlan->stmtListMgr;    \
    (_dst_)->myPlan->bindNode            = (_src_)->myPlan->bindNode;       \
    (_dst_)->propertyEnv                 = (_src_)->propertyEnv;            \
    (_dst_)->prepTemplateHeader          = (_src_)->prepTemplateHeader;     \
    (_dst_)->prepTemplate                = (_src_)->prepTemplate;           \
    (_dst_)->pTmplate                    = (_src_)->pTmplate;               \
    (_dst_)->session                     = (_src_)->session;                \
    (_dst_)->stmtInfo                    = (_src_)->stmtInfo;               \
    (_dst_)->qmeMem                      = (_src_)->qmeMem;                 \
    (_dst_)->qmxMem                      = (_src_)->qmxMem;                 \
    (_dst_)->qmbMem                      = (_src_)->qmbMem;                 \
    (_dst_)->qmtMem                      = (_src_)->qmtMem;                 \
    (_dst_)->qxcMem                      = (_src_)->qxcMem;                 \
    (_dst_)->qmsConvMem                  = (_src_)->qmsConvMem;             \
    (_dst_)->qmpStackPosition            = (_src_)->qmpStackPosition;       \
    (_dst_)->qmeStackPosition            = (_src_)->qmeStackPosition;       \
    (_dst_)->pBindParam                  = NULL;                            \
    (_dst_)->pBindParamCount             = 0;                               \
    (_dst_)->bindParamDataInLastId       = 0;                               \
    (_dst_)->spvEnv                      = (_src_)->spvEnv;                 \
    (_dst_)->spxEnv                      = (_src_)->spxEnv;                 \
    (_dst_)->sharedFlag                  = (_src_)->sharedFlag;             \
    (_dst_)->mStatistics                 = (_src_)->mStatistics;            \
    (_dst_)->stmtMutexPtr                = (_src_)->stmtMutexPtr;           \
    (_dst_)->planTreeFlag                = (_src_)->planTreeFlag;           \
    (_dst_)->calledByPSMFlag             = (_src_)->calledByPSMFlag;        \
    (_dst_)->disableLeftStore            = (_src_)->disableLeftStore;       \
    (_dst_)->mInplaceUpdateDisableFlag   = (_src_)->mInplaceUpdateDisableFlag;\
    (_dst_)->namedVarNode                = (_src_)->namedVarNode;           \
    (_dst_)->mThrMgr                     = (_src_)->mThrMgr;                \
    (_dst_)->mPRLQMemList                = (_src_)->mPRLQMemList;           \
    (_dst_)->simpleInfo                  = (_src_)->simpleInfo;             \
    (_dst_)->mFlag                       = (_src_)->mFlag;                  \
    (_dst_)->mStmtList                   = (_src_)->mStmtList;              \
    (_dst_)->mRandomPlanInfo             = (_src_)->mRandomPlanInfo;        \
}

#define QC_SET_INIT_PARSE_TREE(_dst_, _stmtPos_)                    \
{                                                                   \
    SET_POSITION( ((qcParseTree *)_dst_)->stmtPos, _stmtPos_ );     \
    ((qcParseTree *)(_dst_))->stmtShard   = QC_STMT_SHARD_NONE;     \
    ((qcParseTree *)(_dst_))->currValSeqs = NULL;                   \
    ((qcParseTree *)(_dst_))->nextValSeqs = NULL;                   \
}

/* BUG-37981
   template�� stackBuffer, stack , stackCount , stackRemain
   ���� �� ���� */
#define QC_CONNECT_TEMPLATE_STACK( _detTemplate_ , _oriStackBuffer_ , _oriStack_ , _oriStackCount_ , _oriStackRemain_ )\
{                                                                                                                   \
    _detTemplate_->tmplate.stackBuffer = _oriStackBuffer_;                                                          \
    _detTemplate_->tmplate.stack       = _oriStack_;                                                                \
    _detTemplate_->tmplate.stackCount  = _oriStackCount_;                                                           \
    _detTemplate_->tmplate.stackRemain = _oriStackRemain_;                                                          \
}

#define QC_DISCONNECT_TEMPLATE_STACK( _detTemplate_ )             \
{                                                              \
    _detTemplate_->tmplate.stackBuffer = NULL;                 \
    _detTemplate_->tmplate.stack       = NULL;                 \
    _detTemplate_->tmplate.stackCount  = 0;                    \
    _detTemplate_->tmplate.stackRemain = 0;                    \
}

struct qcStatement;
struct qcTemplate;
struct qcParseTree;
struct qmnPlan;
struct qmoScanDecisionFactor;
struct qsvEnvInfo;
struct qsxEnvInfo;
struct qsxEnvParentInfo;
struct qmoSystemStatistics;
struct qcSessionPkgInfo;           // PROJ-1073 Package
struct qmcThrMgr;
/* PROJ-2451 Concurrent Execute Package */
struct qscConcMgr;
// PROJ-1094 Extend UDT
struct qsxArrayMemMgr;
struct qsxArrayInfo;

// BUG-43158 Enhance statement list caching in PSM
struct qsxStmtList;

typedef IDE_RC (*qcParseFunc)    ( qcStatement * );
typedef IDE_RC (*qcValidateFunc) ( qcStatement * );
typedef IDE_RC (*qcOptimizeFunc) ( qcStatement * );
typedef IDE_RC (*qcExecuteFunc)  ( qcStatement * );

//-----------------------------------------------
// PROJ-1407 Temporary Table
// session temporary table�� �����ϱ����� �ڷᱸ��
//-----------------------------------------------
typedef struct qcTempIndex
{
    UInt           baseIndexID;   // base index id
    const void   * indexHandle;   // cloned temp index handle
#ifdef DEBUG
    UInt           indexID;       // cloned temp index id
#endif
} qcTempIndex;

typedef struct qcTempTable
{
    UInt           baseTableID;  // temp table id
    smSCN          baseTableSCN; // temp table created SCN
    const void   * tableHandle;  // cloned temp table handle
   
    UInt           indexCount;
    qcTempIndex  * indices;      // cloned temp index
} qcTempTable;

typedef struct qcTempTables
{
    qcTempTable  * tables;
    SInt           tableCount;
    SInt           tableAllocCount;
} qcTempTables;

typedef struct qcTemporaryObjInfo
{
    qcTempTables   transTempTable;    // on commit delete rows
    qcTempTables   sessionTempTable;  // on commit preserve rows
} qcTemporaryObjInfo;

//-----------------------------------------------
// [qcUserInfo]
// Statement�� �������� User �� ����
//-----------------------------------------------

typedef struct qcUserInfo
{
    UInt            userID;          // Session�� User ID
    scSpaceID       tableSpaceID;    // User�� Table Space ID
    scSpaceID       tempSpaceID;     // User�� Temp Table Space ID
    idBool          mIsSysdba;
} qcUserInfo;

// SYSTEM USER�� ���� ����
// qcmUser.cpp �� ���ǵ�.
//extern qcUserInfo gQcmSystemUserInfo;

// BUG-41248 DBMS_SQL package
#define QC_MAX_BIND_NAME (128)

typedef struct qcBindInfo
{
    SChar               mName[QC_MAX_BIND_NAME + 1];
    mtcColumn           mColumn;
    SChar             * mValue;
    UInt                mValueSize;
    struct qcBindInfo * mNext;
} qcBindInfo;

typedef struct qcOpenCursorInfo
{
    void       * mMmStatement;
    iduMemory  * mMemory;
    qcBindInfo * mBindInfo;
    idBool       mRecordExist;
    UInt         mFetchCount;
} qcOpenCursorInfo;

typedef struct qcBindNode
{
    qtcNode    * mVarNode;
    qcBindNode * mNext;
} qcBindNode;

// BUG-43158 Enhance statement list caching in PSM
typedef struct qcStmtListInfo
{
    UInt           mStmtListCount;
    UInt           mStmtListCursor;
    UInt           mStmtListFreedCount;

    UInt           mStmtPoolCount;

    qsxStmtList  * mStmtList;
} qcStmtListInfo;

/* BUG-43605 [mt] random�Լ��� seed ������ session ������ �����ؾ� �մϴ�. */
#define QC_RAND_MT_N (624)

typedef struct qcRandomInfo
{
    idBool mExistSeed;
    UInt   mIndex;
    UInt   mMap[QC_RAND_MT_N];
} qcRandomInfo;

// session���� �����ؾ� �ϴ� ����������,
// qp������ ����ϴ� �������� ����.
typedef struct qcSessionSpecific
{
    UInt                       mFlag;   // QC_SESSION_ALTER_META_MASK ����

    // PROJ-1371 PSM File Handling
    struct qcSessionObjInfo  * mSessionObj;

    // sequence cache
    qmxSessionCache            mCache;

    // PROJ-1407 Temporary Table
    qcTemporaryObjInfo         mTemporaryObj;

    // PROJ-1073 Package
    qcSessionPkgInfo         * mSessionPkg;

    // BUG-38129
    scGRID                     mLastRowGRID;

    /* PROJ-2451 Concurrent Execute Package */
    qmcThrMgr                * mThrMgr;
    qscConcMgr               * mConcMgr;

    // PROJ-1904 Extend UDT
    qsxArrayMemMgr           * mArrMemMgr;
    qsxArrayInfo             * mArrInfo;

    // BUG-41248 DBMS_SQL package
    qcOpenCursorInfo         * mOpenCursorInfo;
    // BUG-44856
    UInt                       mLastCursorId;

    // BUG-43158 Enhance statement list caching in PSM
    qcStmtListInfo             mStmtListInfo;

    /* BUG-43605 [mt] random�Լ��� seed ������ session ������ �����ؾ� �մϴ�. */
    qcRandomInfo               mRandomInfo;

    // PROJ-2638
    struct sdiClientInfo     * mClientInfo;

} qcSessionSpecific;

// session ����
typedef struct qcSession
{
    void                * mMmSession;
    qcSessionSpecific     mQPSpecific;

    // BUG-24407
    // ���ο��� ������ session�̶� setSessionUserID�� ȣ��Ǵ�
    // ��찡 �����Ƿ� userID�� ������ �����Ѵ�.
    UInt                  mUserID;

    /*
     * BUG-37093
     * ���ο��� ������ session�̶� MM Session�� �����ϴ� ��찡 �־�
     * ������ �����Ѵ�.
     */
    void                * mMmSessionForDatabaseLink;

} qcSession;

typedef struct qcStmtInfo
{
    // qp���� prepare/execute�� �ʿ��� smiStatement
    smiStatement        * mSmiStmtForExecute;
    void                * mMmStatement;
    idBool                mIsQpAlloc;
} qcStmtInfo;

typedef struct qcNamePosition
{
    SChar * stmtText;  // BUG-18300
    SInt    offset;
    SInt    size;
} qcNamePosition;

// session������ ��� ���� mmcSession�� callback �Լ����� ����.
typedef struct qcSessionCallback
{
    const mtlModule *(*mGetLanguage)(void * aMmSession);
    SChar       *(*mGetDateFormat)(void * aMmSession);
    SChar       *(*mGetUserName)(void * aMmSession);
    SChar       *(*mGetUserPassword)(void * aMmSession);
    UInt         (*mGetUserID)(void * aMmSession);
    void         (*mSetUserID)(void * aMmSession, UInt aUserID);
    UInt         (*mGetSessionID)(void * aMmSession);
    SChar       *(*mGetSessionLoginIP)(void * aMmSession);
    scSpaceID    (*mGetTableSpaceID)(void * aMmSession);
    scSpaceID    (*mGetTempSpaceID)(void * aMmSession);
    idBool       (*mIsSysdbaUser)(void * aMmSession);
    idBool       (*mIsBigEndianClient)(void *aMmSession);
    UInt         (*mGetStackSize)(void * aMmSession);
    UInt         (*mGetNormalFormMaximum)(void * aMmSession);
    UInt         (*mGetOptimizerDefaultTempTbsType)(void * aMmSession);
    UInt         (*mGetOptimizerMode)(void * aMmSession);
    UInt         (*mGetSelectHeaderDisplay)(void * aMmSession);
    IDE_RC       (*mSavepoint)(void        * aMmSession,
                               const SChar * aSavepoint,
                               idBool        aInStoredProc);
    IDE_RC       (*mCommit)(void    * aMmSession,
                            idBool    aInStoredProc);
    IDE_RC       (*mRollback)(void        * aMmSession,
                              const SChar * aSavepoint,
                              idBool        aInStoredProd);
    //PROJ-1541
    IDE_RC       (*mSetReplicationMode)(void * aMmSession,
                                        UInt   aFlagReplicationTx);
    IDE_RC       (*mSetTX)(void * aMmSession,
                           UInt   aType,
                           UInt   aValue,
                           idBool aIsSession );
    IDE_RC       (*mSetStackSize)(void * aMmSession,
                                  SInt   aStackSize );
    IDE_RC       (*mSet)( void  * aMmSession,
                          SChar * aName,
                          SChar * aValue );
    IDE_RC       (*mSetProperty)(void  * aMmSession,
                                 SChar * aPropName,
                                 UInt    aPropNameSize,
                                 SChar * aPropValueStr,
                                 UInt    aPropValueSize );
    void         (*mMemoryCompact)();

    // qsxEnv, qsfPrint���� �����.
    IDE_RC       (*mPrintToClient)(void   * aMmSession,
                                   UChar  * aMessage,
                                   UInt     aMessageLen );

    IDE_RC       (*mGetSvrDN)(void   *  aMmSession,
                              SChar  ** aSvrDN,
                              UInt   *  aSvrDNLen );

    UInt         (*mGetSTObjBufSize)(void * aMmSession);

    /* PROJ-2446: BUG-24361 ST_ALLOW_HINT (XDB ����) �ݿ� */
    UInt         (*mGetSTAllowLev)(void * aMmSession);

    idBool       (*mIsParallelDml)(void * aMmSession);

    /* BUG-20856 XA */
    IDE_RC       (*mCommitForce)(void  * aMmSession,
                                 SChar * aXIDStr,
                                 UInt    aXIDStrSize );

    IDE_RC       (*mRollbackForce)(void  * aMmSession,
                                   SChar * aXIDStr,
                                   UInt    aXIDStrSize );

    //PROJ-1436 SQL-Plan Cache.
    IDE_RC       (*mCompactPlanCache)(void  *aMmStatement);
    IDE_RC       (*mResetPlanCache)  (void  *aMmStatement);

    // PROJ-1579 NCHAR
    UInt         (*mGetNcharLiteralReplace)(void * aMmSession);
    //BUG-21122
    UInt         (*mGetAutoRemoteExec)(void * aMmSession);
    /* PROJ-2446: BUG-24957 plan ��ȸ�� ��ü�� ����� ǥ��(XDB ����) �ݿ� */
    UInt         (*mGetDetailSchema)(void * aMmSession);

    // BUG-25999
    IDE_RC       (*mRemoveXid)(void  * aMmSession,
                               SChar * aXIDStr,
                               UInt    aXIDStrSize );
    // BUG-34830
    UInt         (*mGetTrclogDetailPredicate)(void * aMmSession);

    SInt         (*mGetOptimizerDiskIndexCostAdj)(void * aMmSession);

    // BUG-43736
    SInt         (*mGetOptimizerMemoryIndexCostAdj)(void * aMmSession);

    /* PROJ-2109 : Remove the bottleneck of alloc/free stmts. */
    /* Callback function to obtain a mutex from the mutex pool in mmcSession. */
    IDE_RC       (*mGetMutexFromPool)(void      * aMmSession,
                                      iduMutex ** aMutexObj,
                                      SChar     * aMutexName);
    /* Callback function to free a mutex from the mutex pool in mmcSession. */
    IDE_RC       (*mFreeMutexFromPool)(void     * aMmSession,
                                       iduMutex * aMutexObj);
    SChar      * (*mGetNlsISOCurrency)(void * aMmSession); /* PROJ-2208 */
    SChar      * (*mGetNlsCurrency)(void * aMmSession);    /* PROJ-2208 */
    SChar      * (*mGetNlsNumChar)(void * aMmSession);     /* PROJ-2208 */
    /* BUG-41511 supporting to similar DBMS_APPLICATION_INFO */
    IDE_RC       (*mSetClientAppInfo)( void * aMmSession,
                                       SChar *aClientAppInfo,
                                       UInt   aLength );
    IDE_RC       (*mSetModuleInfo)( void  * aMmSession,
                                    SChar * aModuleInfo,
                                    UInt    aLength );
    IDE_RC       (*mSetActionInfo)( void  * aMmSession,
                                    SChar * aActionInfo,
                                    UInt    aLength );
    /* PROJ-2209 DBTIMEZONE */
    SLong        (*mGetSessionTimezoneSecond)( void * aMmSession );
    SChar       *(*mGetSessionTimezoneString)( void * aMmSession );

    /* PROJ-2047 Strengthening LOB - LOBCACHE */
    UInt         (*mGetLobCacheThreshold)(void  * aMmSession);

    /* PROJ-1090 Function-based Index */
    UInt         (*mGetQueryRewriteEnable)(void  * aMmSession);

    /* PROJ-1832 New database link */
    void       * (*mGetDatabaseLinkSession)( void * aMmSession );

    IDE_RC       (*mCommitForceDatabaseLink)( void      * aMmSession,
                                              idBool      aInStoredProc );

    IDE_RC       (*mRollbackForceDatabaseLink)( void    * aMmSession,
                                                idBool    aInStoredProd );
    // BUG-38430
    ULong        (*mGetSessionLastProcessRow)(void * aMmSession);

    /* BUG-38509 autonomous transaction */
    IDE_RC       (*mSwapTransaction)( void * aUserContext , idBool aIsAT );
    /* PROJ-1812 ROLE */
    UInt       * (*mGetRoleList)(void  * aMmSession);
    
    /* PROJ-2441 flashback */
    UInt         (*mGetRecyclebinEnable)(void  * aMmSession);

    /* BUG-42853 LOCK TABLE�� UNTIL NEXT DDL ��� �߰� */
    idBool       (*mGetLockTableUntilNextDDL)( void * aMmSession );
    void         (*mSetLockTableUntilNextDDL)( void * aMmSession, idBool aValue );
    UInt         (*mGetTableIDOfLockTableUntilNextDDL)( void * aMmSession );
    void         (*mSetTableIDOfLockTableUntilNextDDL)( void * aMmSession, UInt aValue );

    // BUG-41398 use old sort
    UInt         (*mGetUseOldSort)(void  * aMmSession);

    /* PROJ-2451 Concurrent Execute Package */
    IDE_RC       (*mAllocInternalSession)(void ** aMmSession, void * aOrgMmSession);
    IDE_RC       (*mFreeInternalSession)(void * aMmSession, idBool aIsSuccess );
    smiTrans   * (*mGetTrans)(void * aMmSession);
    /* PROJ-2446 ONE SOURCE */
    idvSQL     * (*mGetStatistics)(void  * aMmSession);
    /* BUG-41452 Built-in functions for getting array binding info.*/
    IDE_RC       (*mGetArrayBindInfo)(void * aUserContext); 
    /* BUG-41561 */
    UInt         (*mGetLoginUserID)( void * aMmSession );
    
    // BUG-41944
    UInt         (*mGetArithmeticOpMode)( void * aMmSession );
    /* PROJ-2462 Result Cache */
    UInt         (*mGetResultCacheEnable)(void * aMmSession);
    UInt         (*mGetTopResultCacheMode)(void * aMmSession);
    idBool       (*mIsAutoCommit)(void * aMmSession);
    // PROJ-1904 Extend UDT
    qcSession  * (*mGetQcSession)(void * aMmSession);
    /* PROJ-2492 Dynamic sample selection */
    UInt         (*mGetOptimizerAutoStats)(void * aMmSession);
    /* BUG-42134 Created transitivity predicate of join predicate must be reinforced. */
    UInt         (*mGetOptimizerTransitivityOldRule)(void * aMmSession);

    // BUG-42464 dbms_alert package
    // REGISTER
    IDE_RC      (*mRegister)( void  * aSession,
                              SChar * aName,
                              UShort  aNameSize ); 
    // REMOVE 
    IDE_RC      (*mRemove)( void  * aSession,
                            SChar * aName,
                            UShort  aNameSize ); 
    // REMOVEALL 
    IDE_RC      (*mRemoveAll)( void * aSession ); 
    // SET_DEFAULT 
    IDE_RC      (*mSetDefaults)( void  * aSession,
                                 SInt    aPollingInterval ); 
    // SIGNAL 
    IDE_RC      (*mSignal)( void  * aSession,
                            SChar * aName,
                            UShort  aNameSize,
                            SChar * aMessage,
                            UShort  aMessageSize ); 
    // WAITANY 
    IDE_RC      (*mWaitAny)( void   * aSession,
                             idvSQL * aStatistics,
                             SChar  * aName,
                             UShort * aNameSize,
                             SChar  * aMessage,
                             UShort * aMessageSize,
                             SInt   * aStatus,
                             SInt     aTimeout ); 
    // WAIT
    IDE_RC      (*mWaitOne) ( void   * aSession,
                              idvSQL * aStatistics,
                              SChar  * aName,
                              UShort * aNameSize,
                              SChar  * aMessage,
                              UShort * aMessageSize,
                              SInt   * aStatus,
                              SInt     aTimeout );
    /* BUG-42639 Monitoring query */
    UInt         (*mGetOptimizerPerformanceView)(void * aMmSession);

    /* PROJ-2624 [��ɼ�] MM - ������ access_list ������� ���� */
    IDE_RC       (*mReloadAccessList)();
                                            
    // PROJ-2638
    ULong        (*mGetShardPIN)(void * aMmSession);
    SChar       *(*mGetShardNodeName)(void * aMmSession);
    IDE_RC       (*mSetShardLinker)(void * aMmSession);
    UChar        (*mGetExplainPlan)(void * aMmSession);
} qcSessionCallback;

/*
 * PROJ-1832 New database link.
 *
 * Database Link callback
 */
typedef struct qcDatabaseLinkCallback
{
    IDE_RC (* mStartDatabaseLinker)( void );

    IDE_RC (* mStopDatabaseLinker)( idBool aForceFlag );

    IDE_RC (* mDumpDatabaseLinker)( void );

    IDE_RC (* mCloseDatabaseLinkAll)( idvSQL * aStatistics,
                                      void   * aDkiSession );

    IDE_RC (* mCloseDatabaseLink)( idvSQL * aStatistics,
                                   void   * aDkiSession,
                                   SChar  * aDatabaseLinkName );

    IDE_RC (* mValidateCreateDatabaseLink)( void   * aQcStatement,
                                            idBool   aPublicFlag,
                                            SChar  * aDatabaseLinkName,
                                            SChar  * aUserId,
                                            SChar  * aPassword,
                                            SChar  * aTargetName );

    IDE_RC (* mExecuteCreateDatabaseLink)( void   * aQcStatement,
                                           idBool   aPublicFlag,
                                           SChar  * aDatabaseLinkName,
                                           SChar  * aUserId,
                                           SChar  * aPassword,
                                           SChar  * aTargetName );

    IDE_RC (* mValidateDropDatabaseLink)( void   * aQcStatement,
                                          idBool   aPublicFlag,
                                          SChar  * aDatabaseLinkName );

    IDE_RC (* mExecuteDropDatabaseLink)( void   * aQcStatement,
                                         idBool   aPublicFlag,
                                         SChar  * aDatabaseLinkName );

    IDE_RC (* mDropDatabaseLinkByUserId)( void * aQcStatement,
                                          UInt   aUserId );

    IDE_RC (* mOpenShardConnection)( void * aDataNode );

    void   (* mCloseShardConnection)( void * aDataNode );

    IDE_RC (* mAddShardTransaction)( idvSQL * aStatistics,
                                     smTID    aTransID,
                                     void   * aDataNode );

    void   (* mDelShardTransaction)( void * aDataNode );

} qcDatabaseLinkCallback;

/*
 * PROJ-1832 New database link.
 *
 * Database Link callback for remote table.
 */

typedef struct qcRemoteTableColumnInfo
{
    SChar columnName[ QC_MAX_OBJECT_NAME_LEN + 1 ];

    mtcColumn column;

} qcRemoteTableColumnInfo;

typedef struct qcRemoteTableCallback
{

    IDE_RC (* mGetRemoteTableMeta)(
        void                      * aQcStatement,
        void                      * aDkiSession,
        idBool                      aIsStore,
        SChar                     * aDatabaseLinkName,
        SChar                     * aRemoteQuery,
        void                     ** aRemoteTableHandle,
        SInt                      * aColumnCount,
        qcRemoteTableColumnInfo  ** aColumnInfoArray );

    IDE_RC (* mFreeRemoteTableColumnInfo)(
        qcRemoteTableColumnInfo   * aColumnInfoArray );

    IDE_RC (* mInvalidResultSetMetaCache)( SChar * aDatabaseLinkName,
                                           SChar * aRemoteQuery );

} qcRemoteTableCallback;

//---------------------------------------------------
// BUG-16422
// [qcTableInfoMgr]
// execute�� �ӽ� ������ tableInfo�� ����
//---------------------------------------------------
typedef struct qcTableInfoMgr
{
    void             * tableHandle;
    void             * oldTableInfo;
    void             * newTableInfo;

    qcTableInfoMgr   * next;
} qcTableInfoMgr;


//---------------------------------------------------
// PROJ-1358
// [qcDepInfo]
// ���� ���̺��� ������ ����
//---------------------------------------------------

// PROJ-1358
// �ϳ��� ������ ������ �� �ִ� ���̺��� ����
#define QC_MAX_REF_TABLE_CNT                         (32)

//---------------------------------------------------
// PROJ-1653 Outer Join Operator (+)
//
// joinOper �� validation �� ���� �ڷᱸ���� validation ���Ŀ���
// �������� �ʴ´�.
// joinOper ���� ������ dependency ���̺��� ���� Outer Join Operator ��
// ��� �پ��ִ����� �����Ѵ�.
// �� ���� qmsQuerySet �� SFWGH �� �� ��(from,where,...)��������
// ��Ȯ�� oring �Ǿ� �ֱ� ������, �� ������ �ڷᱸ���� �ִ�
// depInfo.depJoinOper ���� ����ϸ� �ȵȴ�.
//---------------------------------------------------

typedef struct qcDepInfo
{
    UInt   depCount;                            // ���� ���̺� ����
    UShort depend[QC_MAX_REF_TABLE_CNT];        // ���� ���̺� ����
    UChar  depJoinOper[QC_MAX_REF_TABLE_CNT];   // ���� ���̺��� Outer Join Operator(+) ���翩��
} qcDepInfo;

//---------------------------------------------------
// [qcTableMap]
//  Statement�� �����ϴ� ��� Table���� Map ����
//  ������ ���� ó���� ȿ���� ���� Table Map�� �д�.
//  1. Dependency ����
//  2. �ش� Table�� Column�� ���� ���� ����
//  3. Plan Tree�� �ڵ� �籸��
//---------------------------------------------------

typedef struct qcTableMap
{
    ULong             dependency;   // ������ dependency ��
    struct qmsFrom  * from;         // �ش� Table ID�� �ش��ϴ� From ����
} qcTableMap;

//---------------------------------------------------
// PROJ-1413 Simple View Merging
// [qcTupleVarList]
// from ���� relation�� merge�ϱ� ���� ������ unique��
// tuple variable�� �����ؾ� �ϴµ�, �̸� ���� query����
// ����� ��� tuple variable�� ����ؾ� �Ѵ�.
// (�����δ� $$�� �����ϴ� tuple variable�� ����Ѵ�.)
//---------------------------------------------------

typedef struct qcTupleVarList
{
    qcNamePosition    name;
    qcTupleVarList  * next;
} qcTupleVarList;

#define QC_TUPLE_VAR_HEADER        ((SChar*) "$$")
#define QC_TUPLE_VAR_HEADER_SIZE   (2)

/* PROJ-2462 ResultCache */
typedef struct qcComponentInfo
{
    UInt              planID;
    idBool            isVMTR;
    UInt              count;       /* ���� ���̺� ���� */
    UShort          * components; /*  ���� ���̺��� Tuples */
    qcComponentInfo * next;
} qcComponentInfo;

typedef struct qcTableModifyMap
{
    idBool isChecked;
    SLong  modifyCount;
} qcTableModifyMap;

#define QC_RESULT_CACHE_MASK             (0x0001)
#define QC_RESULT_CACHE_DISABLE          (0x0000)
#define QC_RESULT_CACHE_ENABLE           (0x0001)

#define QC_RESULT_CACHE_TOP_MASK         (0x0002)
#define QC_RESULT_CACHE_TOP_FALSE        (0x0000)
#define QC_RESULT_CACHE_TOP_TRUE         (0x0002)

#define QC_RESULT_CACHE_STACK_DEP_MASK   (0x0004)
#define QC_RESULT_CACHE_STACK_DEP_FALSE  (0x0000)
#define QC_RESULT_CACHE_STACK_DEP_TRUE   (0x0004)

#define QC_RESULT_CACHE_DISABLE_MASK     (0x0008)
#define QC_RESULT_CACHE_DISABLE_FALSE    (0x0000)
#define QC_RESULT_CACHE_DISABLE_TRUE     (0x0008)

#define QC_RESULT_CACHE_DATA_ALLOC_MASK  (0x0010)
#define QC_RESULT_CACHE_DATA_ALLOC_FALSE (0x0000)
#define QC_RESULT_CACHE_DATA_ALLOC_TRUE  (0x0010)

#define QC_RESULT_CACHE_AUTOCOMMIT_MASK  (0x0020)
#define QC_RESULT_CACHE_AUTOCOMMIT_FALSE (0x0000)
#define QC_RESULT_CACHE_AUTOCOMMIT_TRUE  (0x0020)

#define QC_RESULT_CACHE_MAX_EXCEED_MASK  (0x0040)
#define QC_RESULT_CACHE_MAX_EXCEED_FALSE (0x0000)
#define QC_RESULT_CACHE_MAX_EXCEED_TRUE  (0x0040)

#define QC_RESULT_CACHE_INIT( _resultCache_ )        \
{                                                    \
    (_resultCache_)->flag          = 0;              \
    (_resultCache_)->transID       = SM_NULL_TID;    \
    (_resultCache_)->count         = 0;              \
    (_resultCache_)->stack         = NULL;           \
    (_resultCache_)->list          = NULL;           \
    (_resultCache_)->modifyMap     = NULL;           \
    (_resultCache_)->data          = NULL;           \
    (_resultCache_)->dataFlag      = NULL;           \
    (_resultCache_)->bindValues    = NULL;           \
    (_resultCache_)->isBindChanged = ID_FALSE;       \
    (_resultCache_)->memSizeArray  = NULL;           \
    (_resultCache_)->qrcMemMaximum = 0;              \
    (_resultCache_)->qmxMemMaximum = 0;              \
    (_resultCache_)->qmxMemSize    = 0;              \
}

typedef struct qcResultCache
{
    UShort              flag;         // Result Cache�� ���Ǵ� flag
    smTID               transID;      // Transction ID�� �����س��� AutoCommit Off�� check
    UInt                count;        // ��ü Query�� ���� cache count
    qcComponentInfo   * stack;        // ComponentInfo Stack
    qcComponentInfo   * list;         // ComponentInfo List
    qcTableModifyMap  * modifyMap;    // ������ ���� Table����ModifyCount Map
    UChar             * data;         // Result Cache�� Data����
    UInt              * dataFlag;     // Result Cache�� Data Flag ����
    void              * bindValues;   // Bind Value�� ������ ���´�.
    idBool              isBindChanged;// Bind ������ �ٲ�������� üũ
    iduMemory        ** memArray;     // ������ Cache�� ����ϴ� Memory �迭
    ULong             * memSizeArray; // ������ Cache�� ����ϴ� Memory ũ�� �迭
    ULong               qrcMemMaximum;// QCU_RESULT_CACHE_MEMORY_MAXIMUM ũ��
    ULong               qmxMemMaximum;// iduProperty::getExecuteMemoryMax  ũ��
    ULong               qmxMemSize;   // execution Memory Maxüũ�� ���� qmxMemSize
} qcResultCache;

typedef struct qcShardExecData
{
    UInt       planCount;
    UChar    * execInfo;
    UInt       dataSize;
    UChar    * data;
} qcShardExecData;

//---------------------------------------------------
// PROJ-1436 SQL-Plan Cache
// [qcgPlanProperty]
// plan ������ ������ ��� system & session property
//---------------------------------------------------

enum qcPlanPropertyKind
{
    PLAN_PROPERTY_USER_ID = 0,
    PLAN_PROPERTY_OPTIMIZER_MODE,
    PLAN_PROPERTY_HOST_OPTIMIZE_ENABLE,
    PLAN_PROPERTY_SELECT_HEADER_DISPLAY,
    PLAN_PROPERTY_STACK_SIZE,
    PLAN_PROPERTY_NORMAL_FORM_MAXIMUM,
    PLAN_PROPERTY_DEFAULT_DATE_FORMAT,
    PLAN_PROPERTY_ST_OBJECT_SIZE,
    PLAN_PROPERTY_OPTIMIZER_SIMPLE_VIEW_MERGE_DISABLE,
    PLAN_PROPERTY_OPTIMIZER_DEFAULT_TEMP_TBS_TYPE,
    PLAN_PROPERTY_FAKE_TPCH_SCALE_FACTOR,
    PLAN_PROPERTY_FAKE_BUFFER_SIZE,
    PLAN_PROPERTY_OPTIMIZER_DISK_INDEX_COST_ADJ,
    PLAN_PROPERTY_OPTIMIZER_MEMORY_INDEX_COST_ADJ,
    PLAN_PROPERTY_OPTIMIZER_HASH_JOIN_COST_ADJ,
    PLAN_PROPERTY_OPTIMIZER_SUBQUERY_OPTIMIZE_METHOD,
    PLAN_PROPERTY_OPTIMIZER_ANSI_JOIN_ORDERING,
    PLAN_PROPERTY_OPTIMIZER_ANSI_INNER_JOIN_CONVERT,
    PLAN_PROPERTY_NLS_CURRENCY_FORMAT,
    PLAN_PROPERTY_OPTIMIZER_REFINE_PREPARE_MEMORY,
    PLAN_PROPERTY_OPTIMIZER_PARTIAL_NORMALIZE,    // BUG-35155
    PLAN_PROPERTY_QUERY_REWRITE_ENABLE,
    PLAN_PROPERTY_OPTIMIZER_UNNEST_SUBQUERY,
    PLAN_PROPERTY_OPTIMIZER_UNNEST_COMPLEX_SUBQUERY,
    PLAN_PROPERTY_OPTIMIZER_UNNEST_AGGREGATION_SUBQUERY,
    PLAN_PROPERTY_OPTIMIZER_ELIMINATE_COMMON_SUBEXPRESSION,
    PLAN_PROPERTY_OPTIMIZER_CONSTANT_FILTER_SUBSUMPTION,
    PLAN_PROPERTY_OPTIMIZER_FEATURE_ENABLE,
    PLAN_PROPERTY_SYS_CONNECT_BY_PATH_PRECISION,
    PLAN_PROPERTY_GROUP_CONCAT_PRECISION,
    PLAN_PROPERTY_OPTIMIZER_USE_TEMP_TYPE,
    PLAN_PROPERTY_AVERAGE_TRANSFORM_ENABLE,
    PLAN_PROPERTY_OPTIMIZER_FIXED_GROUP_MEMORY_TEMP,
    PLAN_PROPERTY_OPTIMIZER_OUTERJOIN_ELIMINATION,
    PLAN_PROPERTY_OPTIMIZER_DNF_DISABLE,
    PLAN_PROPERTY_OPTIMIZER_COUNT_COLUMN_TO_COUNT_ASTAR,
    PLAN_PROPERTY_OPTIMIZER_JOIN_DISABLE,
    PLAN_PROPERTY_OPTIMIZER_VIEW_TARGET_ENABLE,
    PLAN_PROPERTY_OPTIMIZER_ORDER_BY_ELIMINATION_ENABLE,
    PLAN_PROPERTY_OPTIMIZER_DISTINCT_ELIMINATION_ENABLE,
    PLAN_PROPERTY_ARITHMETIC_OP_MODE,
    PLAN_PROPERTY_RESULT_CACHE_ENABLE,
    PLAN_PROPERTY_TOP_RESULT_CACHE_MODE,
    PLAN_PROPERTY_EXECUTOR_FAST_SIMPLE_QUERY,
    PLAN_PROPERTY_OPTIMIZER_LIST_TRANSFORMATION,
    PLAN_PROPERTY_OPTIMIZER_AUTO_STATS,
    PLAN_PROPERTY_OPTIMIZER_TRANSITIVITY_OLD_RULE,
    PLAN_PROPERTY_OPTIMIZER_PERFORMANCE_VIEW,
    PLAN_PROPERTY_OPTIMIZER_INNER_JOIN_PUSH_DOWN,
    PLAN_PROPERTY_OPTIMIZER_ORDER_PUSH_DOWN,
    PLAN_PROPERTY_OPTIMIZER_TARGET_SUBQUERY_UNNEST_DISABLE,
    PLAN_PROPERTY_OPTIMIZER_TARGET_SUBQUERY_REMOVAL_DISABLE,
    PLAN_PROPERTY_OPTIMIZER_DELAYED_EXECUTION,
    PLAN_PROPERTY_OPTIMIZER_LIKE_INDEX_SCAN_WITH_OUTER_COLUMN_DISABLE,
    PLAN_PROPERTY_OPTIMIZER_PLAN_RANDOM_SEED,
    PLAN_PROPERTY_OPTIMIZER_HIERARCHY_TRANSFORMATION,
    PLAN_PROPERTY_OPTIMIZER_DBMS_STAT_POLICY,
    PLAN_PROPERTY_OPTIMIZER_INDEX_NL_JOIN_ACCESS_METHOD_POLICY,  /* BUG-44850 */
    PLAN_PROPERTY_OPTIMIZER_SEMI_JOIN_REMOVE,
    PLAN_PROPERTY_SHARD_LINKER_CHANGE_NUMBER,
    PLAN_PROPERTY_SHARD_AGGREGATION_TRANSFORM_DISABLE,
    PLAN_PROPERTY_MAX
};

typedef struct qcPlanProperty
{
    idBool  userIDRef;
    UInt    userID;

    idBool  optimizerModeRef;
    UInt    optimizerMode;

    idBool  hostOptimizeEnableRef;
    UInt    hostOptimizeEnable;

    idBool  selectHeaderDisplayRef;
    UInt    selectHeaderDisplay;

    idBool  stackSizeRef;
    UInt    stackSize;

    idBool  normalFormMaximumRef;
    UInt    normalFormMaximum;

    idBool  defaultDateFormatRef;
    SChar   defaultDateFormat[IDP_MAX_VALUE_LEN];  // MTC_TO_CHAR_MAX_PRECISION ���� ũ��.

    idBool  STObjBufSizeRef;
    UInt    STObjBufSize;

    idBool  optimizerSimpleViewMergingDisableRef;
    UInt    optimizerSimpleViewMergingDisable;

    // BUG-23780 TEMP_TBS_MEMORY ��Ʈ ���뿩�θ� property�� ����
    idBool  optimizerDefaultTempTbsTypeRef;
    UInt    optimizerDefaultTempTbsType;

    idBool  fakeTpchScaleFactorRef;
    UInt    fakeTpchScaleFactor;

    idBool  fakeBufferSizeRef;
    UInt    fakeBufferSize;

    idBool  optimizerDiskIndexCostAdjRef;
    SInt    optimizerDiskIndexCostAdj;

    // BUG-43736
    idBool  optimizerMemoryIndexCostAdjRef;
    SInt    optimizerMemoryIndexCostAdj;

    // BUG-34441
    idBool  optimizerHashJoinCostAdjRef;
    UInt    optimizerHashJoinCostAdj;

    idBool  optimizerSubqueryOptimizeMethodRef;
    UInt    optimizerSubqueryOptimizeMethod;

    idBool  optimizerAnsiJoinOrderingRef;
    UInt    optimizerAnsiJoinOrdering;

    idBool  optimizerAnsiInnerJoinConvertRef;
    UInt    optimizerAnsiInnerJoinConvert;

    /* PROJ-2208 Multi Curency */
    idBool  nlsCurrencyRef;
    SChar   nlsISOCurrency[MTL_TERRITORY_ISO_LEN + 1];
    SChar   nlsCurrency[MTL_TERRITORY_CURRENCY_LEN + 1];
    SChar   nlsNumericChar[MTL_TERRITORY_NUMERIC_CHAR_LEN + 1];

    // BUG-34350
    idBool  optimizerRefinePrepareMemoryRef;
    UInt    optimizerRefinePrepareMemory;

    // BUG-35155 Partial CNF
    idBool  optimizerPartialNormalizeRef;
    UInt    optimizerPartialNormalize;

    /* PROJ-1090 Function-based Index */
    idBool  queryRewriteEnableRef;
    UInt    queryRewriteEnable;

    // PROJ-1718
    idBool  optimizerUnnestSubqueryRef;
    UInt    optimizerUnnestSubquery;

    idBool  optimizerUnnestComplexSubqueryRef;
    UInt    optimizerUnnestComplexSubquery;

    idBool  optimizerUnnestAggregationSubqueryRef;
    UInt    optimizerUnnestAggregationSubquery;

    /* PROJ-2242 CSE, FS, Feature enable */
    idBool  optimizerEliminateCommonSubexpressionRef;
    UInt    optimizerEliminateCommonSubexpression;

    idBool  optimizerConstantFilterSubsumptionRef;
    UInt    optimizerConstantFilterSubsumption;

    idBool  optimizerFeatureEnableRef;
    SChar   optimizerFeatureEnable[IDP_MAX_VALUE_LEN];

    // BUG-37247
    idBool  sysConnectByPathPrecisionRef;
    UInt    sysConnectByPathPrecision;

    // BUG-37247
    idBool  groupConcatPrecisionRef;
    UInt    groupConcatPrecision;

    // PROJ-2362
    idBool  reduceTempMemoryEnableRef;
    UInt    reduceTempMemoryEnable;

    /* PROJ-2361 */
    idBool  avgTransformEnableRef;
    UInt    avgTransformEnable;

    // BUG-38132
    idBool  fixedGroupMemoryTempRef;
    UInt    fixedGroupMemoryTemp;

    // BUG-38339 Outer Join Elimination
    idBool  outerJoinEliminationRef;
    UInt    outerJoinElimination;

    // BUG-38434
    idBool  optimizerDnfDisableRef;
    UInt    optimizerDnfDisable;

    // BUG-38416 count(column) to count(*)
    idBool  countColumnToCountAStarRef;
    UInt    countColumnToCountAStar;

    // BUG-40022
    idBool  optimizerJoinDisableRef;
    UInt    optimizerJoinDisable;

    // PROJ-2469
    idBool  optimizerViewTargetEnableRef;
    UInt    optimizerViewTargetEnable;

    // BUG-41183 ORDER BY Elimination
    idBool  optimizerOrderByEliminationEnableRef;
    UInt    optimizerOrderByEliminationEnable;

    // BUG-41249 DISTINCT Elimination
    idBool  optimizerDistinctEliminationEnableRef;
    UInt    optimizerDistinctEliminationEnable;

    // BUG-41944
    idBool  arithmeticOpModeRef;
    mtcArithmeticOpMode arithmeticOpMode;

    /* PROJ-2462 Result Cache */
    idBool  resultCacheEnableRef;
    UInt    resultCacheEnable;
    /* PROJ-2462 Result Cache */
    idBool  topResultCacheModeRef;
    UInt    topResultCacheMode;
    idBool  resultCacheMemoryMaximumRef;
    ULong   resultCacheMemoryMaximum;

    /* PROJ-2616 */
    idBool  executorFastSimpleQueryRef;
    UInt    executorFastSimpleQuery;
    
    // BUG-36438 LIST transformation
    idBool  optimizerListTransformationRef;
    UInt    optimizerListTransformation;

    /* PROJ-2492 Dynamic sample selection */
    idBool  mOptimizerAutoStatsRef;
    UInt    mOptimizerAutoStats;

    /* BUG-42134 Created transitivity predicate of join predicate must be reinforced. */
    idBool  optimizerTransitivityOldRuleRef;
    UInt    optimizerTransitivityOldRule;

    /* BUG-42639 Monitoring query */
    idBool  optimizerPerformanceViewRef;
    UInt    optimizerPerformanceView;

    // BUG-43039 inner join push down
    idBool  optimizerInnerJoinPushDownRef;
    UInt    optimizerInnerJoinPushDown;

    // BUG-43068 Indexable order by ����
    idBool  optimizerOrderPushDownRef;
    UInt    optimizerOrderPushDown;

    // BUG-43059 Target subquery unnest/removal disable
    idBool  optimizerTargetSubqueryUnnestDisableRef;
    UInt    optimizerTargetSubqueryUnnestDisable;

    idBool  optimizerTargetSubqueryRemovalDisableRef;
    UInt    optimizerTargetSubqueryRemovalDisable;

    // BUG-43493 delayed execution
    idBool  optimizerDelayedExecutionRef;
    UInt    optimizerDelayedExecution;

    /* BUG-43495 */
    idBool optimizerLikeIndexScanWithOuterColumnDisableRef;
    UInt   optimizerLikeIndexScanWithOuterColumnDisable;

    /* TASK-6744 */
    idBool optimizerPlanRandomSeedRef;
    UInt   optimizerPlanRandomSeed;

    /* PROJ-2641 Hierarchy Query Index */
    idBool mOptimizerHierarchyTransformationRef;
    UInt   mOptimizerHierarchyTransformation;

    // BUG-44795
    idBool optimizerDBMSStatPolicyRef;
    UInt   optimizerDBMSStatPolicy;

    /* BUG-44850 Index NL , Inverse index NL ���� ����ȭ ����� ����� �����ϸ� primary key�� �켱������ ����. */
    idBool optimizerIndexNLJoinAccessMethodPolicyRef;
    UInt   optimizerIndexNLJoinAccessMethodPolicy;
    
    idBool optimizerSemiJoinRemoveRef;
    UInt   optimizerSemiJoinRemove;

    /* BUG-44710 */
    idBool mShardLinkerChangeNumberRef;
    UInt   mShardLinkerChangeNumber;

    /* PROJ-2687 */
    idBool mShardAggregationTransformDisableRef;
    UInt   mShardAggregationTransformDisable;

} qcPlanProperty;

typedef struct qcTemplate
{
    mtcTemplate        tmplate;

    // PROJ-1358
    // Internal Tuple�� �ڵ� Ȯ������ ���� �Բ� Ȯ��Ǿ�� ��.
    qcTableMap       * tableMap;     // Table Map

    UInt               planCount;
    UInt             * planFlag;     // ���� �������� �Ҵ� �� �ʱ�ȭ�Ǿ�� ��.

    qmcCursor        * cursorMgr;    // ���� ó���� ������ Cursor ����
    qmcdTempTableMgr * tempTableMgr; // ���� ó���� ������ Temp Table ����
    qcTableInfoMgr   * tableInfoMgr; // ���� ó���� ������ tableInfo ����

    vSLong             numRows;      // for NON-SELECT DML

    // BUG-44536 Affected row count�� fetched row count�� �и��մϴ�.
    qciStmtType        stmtType;

    UInt               fixedTableAutomataStatus;

    qcStatement      * stmt;         // parent pointer
    UInt               flag;
    UInt               smiStatementFlag; // for SMI_STATEMENT_CURSOR_MASK

    UInt               insOrUptStmtCount;
    UInt             * insOrUptRowValueCount;
    smiValue        ** insOrUptRow;  // for inserting or updating row

    /* PROJ-2209 DBTIMEZONE */
    qtcNode          * unixdate;
    qtcNode          * sysdate;
    qtcNode          * currentdate;

    UInt               tupleVarGenNumber; // PROJ-1413
    qcTupleVarList   * tupleVarList;      // query���� ����� tuple variable�� ���

    idBool             indirectRef;  // PROJ-1436 ���� ���� ����(��ü)�� ǥ��

    /* PROJ-2448 Subquery caching */
    UInt               forceSubqueryCacheDisable;  // QCU_FORCE_SUBQUERY_CACHE_DISABLE ���

    /* PROJ-2452 Caching for DETERMINISTIC Function */
    UInt               cacheMaxCnt;     // QCU_QUERY_EXECUTION_CACHE_MAX_COUNT    ���
    UInt               cacheMaxSize;    // QCU_QUERY_EXECUTION_CACHE_MAX_SIZE     ���
    UInt               cacheBucketCnt;  // QCU_QUERY_EXECUTION_CACHE_BUCKET_COUNT ���
    UInt               cacheObjCnt;     // ���� ���� cache object count
    qtcCacheObj      * cacheObjects;    // cache object array

    /* PROJ-2553 Cache-aware Memory Hash Temp Table */
    idBool             memHashTempPartDisable;     // QCU_HSJN_MEM_TEMP_PARTITIONING_DISABLE   ���
    idBool             memHashTempManualBucketCnt; // QCU_HSJN_MEM_TEMP_AUTO_BUCKETCNT_DISABLE ���
    UInt               memHashTempTlbCount;        // QCU_HSJN_MEM_TEMP_TLB_COUNT              ���

    /* PROJ-2462 Result Cache */
    qcResultCache      resultCache;

    // BUG-44710
    qcShardExecData    shardExecData;


    // BUG-44795
    UInt               optimizerDBMSStatPolicy;
} qcTemplate;

//---------------------------------------------------
// PROJ-1436 SQL-Plan Cache
// [qcPrepTemplate]
// ������ plan�� execute�ϱ� ���ؼ��� private template��
// �����ؾ� �Ѵ�. prepared private template�� �̸� ������
// private template�� ����Ͽ� clone template�� ���� �ʰ�
// �ٷ� ����ϰ� �Ͽ� ������ ��� ��Ų��.
//
// [qcPrepTemplateHeader]
// prepared private template�� property�� ���� �̸� ������
// ������ ������ �� ������ �׺��� �� ����� ���Ǵ� ���
// �߰� ������ private template�� qcPrepTemplateHeader����
// �����Ͽ� ������ �� �ֵ��� �Ѵ�.
//---------------------------------------------------

typedef struct qcPrepTemplate
{
    iduListNode               prepListNode;
    iduVarMemList           * pmtMem;    // template Memory

    qcTemplate              * tmplate;
    struct qciBindParamInfo * bindParam;
} qcPrepTemplate;

typedef struct qcPrepTemplateHeader
{
    /* fix BUG-29965 SQL Plan Cache���� plan execution template ������
       Dynamic SQL ȯ�濡���� ������ �ʿ��ϴ�.
       template �Ҵ�, ������ list full scan�� ���� �ϱ� ���Ͽ�
       ������ ����  free list, used list�� �и��Ѵ�.
     */
    iduMutex           prepMutex;
    iduList            freeList;
    iduList            usedList;
    iduVarMemList    * pmhMem;        // header Memory
    UInt               freeCount;

} qcPrepTemplateHeader;

//---------------------------------------------------
// PROJ-2206 With clause
// with �ڷᱸ��
//---------------------------------------------------
typedef struct qcWithStmt
{
    qcNamePosition        stmtName;
    qcNamePosition        stmtText;

    // PROJ-2582 recursive with
    struct qcmColumn    * columns;        // column alias
    qcStatement         * stmt;
    idBool                isRecursiveView;
    idBool                isTop;
    struct qcmTableInfo * tableInfo;       // recursive view�� ���� tableInfo
    UInt                  tableID;        // stmt�� �Ҵ�� table id

    qcWithStmt          * next;
} qcWithStmt;

typedef struct qcStmtListMgr
{
    // PROJ-2415 Grouping Sets Statement
    // Re-Parsing�� Host Variable������ ���� With�� ��� �ϴ� �Ϳ���
    // �������� �ٲٱ� ���� qcWithStmtListMgr -> qcStmtListMgr �̸� ����

    // For With
    qcWithStmt     * head;           // withStmt ã�� ���� head
    qcWithStmt     * current;        // withStmt ã�� ����
    UInt             tableIDSeqNext; // ������ �Ҵ��� table id

    // For HostVariable    
    SInt             hostVarOffset[ MTC_TUPLE_COLUMN_ID_MAXIMUM ];
    
} qcStmtListMgr;

// BUG-36986
typedef struct qcNamedVarNode
{
    qtcNode          * varNode;      // PSM ���� �ߺ��� ������ variable node
    qcNamedVarNode   * next;
} qcNamedVarNode;

// PROJ-2551 simple query ����ȭ
typedef struct qcSimpleInfo
{
    iduMemoryStatus           status;    // qmxMem status
    struct qcSimpleResult   * results;   // result sets
    vSLong                    numRows;   // affected row count
    UInt                      count;     // select row count
} qcSimpleInfo;

//---------------------------------------------------
// PROJ-1436 SQL-Plan Cache
// [qcSharedPlan]
// plan�� �����ϱ� ���� qcStatement���� plan���ν�
// �����ؾ��ϴ� ������ �����Ѵ�.
//---------------------------------------------------

typedef struct qcOffset
{
    SInt            mOffset;
    SInt            mLen;
    qcOffset      * mNext;
} qcOffset;

// qcSharedPlan.mPlanFlag

#define QC_PLAN_FLAG_INIT                     (0x00000000)

// BUG-43524 hint ������ ������ ������ �˼� ������ ���ڽ��ϴ�.
#define QC_PLAN_HINT_PARSE_MASK               (0x00000001)
#define QC_PLAN_HINT_PARSE_SUCCESS            (0x00000000)
#define QC_PLAN_HINT_PARSE_FAIL               (0x00000001)

typedef struct qcSharedPlan
{
    // plan environment
    struct qcgPlanEnv        * planEnv;    // alloc from qmpMem

    // parsing text information
    SChar                    * stmtText;
    SInt                       stmtTextLen;

    UInt                       mPlanFlag;
    qcOffset                 * mHintOffset;

    /* PROJ-2550 PSM Encryption
       encrypted text�� ������ ù��° parsing ��
       encrypted text�� decryption�� ��
       qss::setStmtText���� ���õȴ�. */
    SChar                    * encryptedText;
    SInt                       encryptedTextLen;

    iduVarMemList            * qmpMem;     // plan Memory
    iduVarMemList            * qmuMem;     // plan backup Memory

    qcParseTree              * parseTree;  // alloc from qmpMem
    struct qmsHints          * hints;      // alloc from qmpMem
    qmnPlan                  * plan;       // alloc from qmpMem
    struct qmgGraph          * graph;      // alloc from qmpMem
    qmoScanDecisionFactor    * scanDecisionFactors; // alloc from qmpMem
    struct qsxProcPlanList   * procPlanList; // alloc from qmpMem

    /* PROJ-2598 Shard pilot(shard Analyze) */
    struct sdiAnalyzeInfo    * mShardAnalysis;
    UShort                     mShardParamOffset;
    UShort                     mShardParamCount;

    // ���� template
    qcTemplate               * sTmplate;    // alloc from qmpMem

    // BUG-20652
    struct qciBindColumnInfo * sBindColumn;      // bind column (qmpMem)
    UShort                     sBindColumnCount;

    // PROJ-1558
    struct qciBindParamInfo  * sBindParam;       // bind parameter (qmpMem)
    UShort                     sBindParamCount;

    // PROJ-2206
    qcStmtListMgr            * stmtListMgr;

    // BUG-41248 DBMS_SQL package
    qcBindNode               * bindNode;
} qcSharedPlan;

typedef struct qcStatement
{
    // PROJ-1436 shared plan
    qcSharedPlan               privatePlan;  // qcStatement�� plan
    qcSharedPlan               sharedPlan;   // ������ plan
    qcSharedPlan             * myPlan;       // privatePlan or sharedPlan
    qcPlanProperty           * propertyEnv;  // alloc from qmeMem (for soft-prepare)

    // PROJ-1436 prepared private template
    qcPrepTemplateHeader     * prepTemplateHeader;  // alloc from pmhMem
    qcPrepTemplate           * prepTemplate;        // alloc from pmtMem

    // private template
    qcTemplate               * pTmplate;    // alloc from qmeMem or prepared template

    qcSession                * session;
    qcStmtInfo               * stmtInfo;

    //fix PROJ-1596
    //qmpMem, qmbMem   -> iduVarMemList
    //qmpStackPosition -> iduVarMemListStatus
    // memory
    iduVarMemList            * qmeMem;      // Prepare Memory
    iduMemory                * qmxMem;      // Execution Memory
    iduVarMemList            * qmbMem;      // Bind Memory
    iduMemory                * qmtMem;      // Trigger template memory
    iduMemory                * qxcMem;      // PROJ-2452 Execution Cache Memory
    iduMemory                * qmsConvMem;  // server conversion memory

    iduVarMemListStatus        qmpStackPosition;  // Stack Position to clear
    iduVarMemListStatus        mQmpStackPositionForCheckInFailure;
    iduVarMemListStatus        qmeStackPosition;  // Stack Position to clear

    // PROJ-1558
    struct qciBindParamInfo  * pBindParam;       // bind parameter (qmeMem)
    UShort                     pBindParamCount;
    UShort                     bindParamDataInLastId;    // PROJ-1697
    // PROJ-2163 : add flag of pBindParam change
    idBool                     pBindParamChangedFlag;

    // stored procedure environment
    qsvEnvInfo               * spvEnv;   // for validation
    qsxEnvInfo               * spxEnv;   // for execution

    /* PROJ-2197 PSM Renewal
     * PSM�� return bulk into�� ó���ϱ� ���ؼ� �߰� */
    qsxEnvParentInfo         * parentInfo;

    // To fix PR-4073 check memory
    iduMutex                   stmtMutex;

    /* BUG-38290
     * SM cursor open �� addOpenedCursor ���� ���� ���ü� ��� ���� mutex
     */
    // BUG-38932 alloc, free cost of cursor mutex is too expensive.
    // qmcCursor �� �ִ� mutex �� qcStatement �� �ű��.
    iduMutex                   mCursorMutex;

    /* PROJ-2109 : Remove the bottleneck of alloc/free stmts. */
    /* This pointer of a mutex will be set either from the mutex pool in mmcSession
       or just the pointer of the stmtMutex. */
    iduMutex                 * stmtMutexPtr;
    /* A flag which indicates whether the stmtMutexPtr was set from the mutex pool or not. */
    idBool                     mutexpoolFlag;
    idBool                     allocFlag;
    idBool                     planTreeFlag;
    idBool                     sharedFlag;
    idBool                     calledByPSMFlag;
    idBool                     disableLeftStore;
    idBool                     mInplaceUpdateDisableFlag;

    idvSQL                   * mStatistics; /* ��� ����*/
    qmoSystemStatistics      * mSysStat; // PROJ-2242

    // BUG-36986
    qcNamedVarNode           * namedVarNode;

    /* PROJ-1071 Parallel Query*/
    qmcThrMgr                * mThrMgr;

    /* BUG-38294 */
    iduList                  * mPRLQMemList;
    iduList                  * mPRLQChdTemplateList;

    // PROJ-2551 simple query ����ȭ
    qcSimpleInfo               simpleInfo;

    UInt                       mFlag;

    qsxStmtList              * mStmtList;

    /* TASK-6744 */
    struct qmgRandomPlanInfo * mRandomPlanInfo;
} qcStatement;

#define QC_SMI_STMT( _QcStmt_ )  ( ( _QcStmt_ )->stmtInfo->mSmiStmtForExecute )
#define QC_MM_STMT( _QcStmt_ )   ( ( _QcStmt_ )->stmtInfo->mMmStatement )
#define QC_QSX_ENV( _QcStmt_ )   ( ( _QcStmt_ )->spxEnv )
#define QC_QME_MEM( _QcStmt_ )   ( ( _QcStmt_ )->qmeMem )
#define QC_QMX_MEM( _QcStmt_ )   ( ( _QcStmt_ )->qmxMem )
#define QC_QMB_MEM( _QcStmt_ )   ( ( _QcStmt_ )->qmbMem )
#define QC_QMT_MEM( _QcStmt_ )   ( ( _QcStmt_ )->qmtMem )
#define QC_QXC_MEM( _QcStmt_ )   ( ( _QcStmt_ )->qxcMem )
#define QC_QMP_MEM( _QcStmt_ )   ( ( _QcStmt_ )->myPlan->qmpMem )
#define QC_QMU_MEM( _QcStmt_ )   ( ( _QcStmt_ )->myPlan->qmuMem )
#define QC_SHARED_TMPLATE( _QcStmt_ )    ( ( _QcStmt_ )->myPlan->sTmplate )
#define QC_PRIVATE_TMPLATE( _QcStmt_ )   ( ( _QcStmt_ )->pTmplate )
#define QC_PARSETREE( _QcStmt_ )   ( ( _QcStmt_ )->myPlan->parseTree )
#define QC_STATISTICS( _QcStmt_ )   ( ( _QcStmt_ )->mStatistics )
#define QC_SMI_STMT_SESSION_IS_JOB( _QcStmt_ )                  \
        (                                                       \
            ( ( ( _QcStmt_ )->session->mQPSpecific.mFlag        \
                & QC_SESSION_INTERNAL_JOB_MASK )                \
              == QC_SESSION_INTERNAL_JOB_TRUE )                 \
            ? ID_TRUE : ID_FALSE                                \
        )

#define QC_SESSION_IS_INTERNAL_EXEC( _QcStmt_ )            \
        (                                                       \
            ( ( ( _QcStmt_ )->session->mQPSpecific.mFlag        \
                & QC_SESSION_INTERNAL_EXEC_MASK )               \
              == QC_SESSION_INTERNAL_EXEC_TRUE )                \
            ? ID_TRUE : ID_FALSE                                \
        )

// PROJ-2446
#define QC_STMTTEXT( _QcStmt_ )    ( ( _QcStmt_ )->myPlan->stmtText )
#define QC_STMTTEXTLEN( _QcStmt_ ) ( ( _QcStmt_ )->myPlan->stmtTextLen )

typedef struct qcParseSeqCaches
{
    void                * sequenceHandle;
    smOID                 sequenceOID;

    qtcNode             * sequenceNode;
    // This node is a representative node
    //      for getting tupleRowID and setting error position.

    // PROJ-2365 sequence table ����
    idBool                sequenceTable;
    struct qcmTableInfo * tableInfo;
    void                * tableHandle;
    smSCN                 tableSCN;

    qcParseSeqCaches    * next;
} qcParseSeqCaches;

// PROJ-1579 NCHAR
typedef struct qcNamePosList
{
    qcNamePosition   namePos;
    qcNamePosList  * next;
} qcNamePosList;

// BUG-44710
typedef enum qcShardStmtType
{
    QC_STMT_SHARD_NONE = 0,
    QC_STMT_SHARD_ANALYZE,  // �м������ ���� ����
    QC_STMT_SHARD_META,     // meta node���� ����
    QC_STMT_SHARD_DATA      // data nodes���� ����
} qcShardStmtType;

// BUG-45359
typedef struct qcShardNodes
{
    qcNamePosition   namePos;
    qcShardNodes   * next;
} qcShardNodes;

typedef struct qcShardStmtSpec
{
    qcNamePosition    position;
    qcShardStmtType   shardType;
    qcShardNodes    * nodes;
} qcShardStmtSpec;

typedef struct qcParseTree
{
    qciStmtType           stmtKind;
    qcNamePosition        stmtPos;
    qcShardStmtType       stmtShard;        // PROJ-2638

    qcParseSeqCaches    * currValSeqs;
    qcParseSeqCaches    * nextValSeqs;

    // function pointer
    qcParseFunc           parse;
    qcValidateFunc        validate;
    qcOptimizeFunc        optimize;
    qcExecuteFunc         execute;

    qcStatement         * stmt;     // parent pointer
    qcShardNodes        * nodes;

} qcParseTree;

// PROJ-1371 PSM File Handling

typedef struct qcFileNode
{
    FILE* fp;
    struct qcFileNode* next;
} qcFileNode;

typedef struct qcFileList
{
    UInt count;
    qcFileNode* fileNode;
} qcFileList;

// BUG-40854
typedef struct qcConnectionNode
{
    PDL_SOCKET socket;
    SLong      connectNodeKey;
    /* PROJ-2657 UTL_SMTP ���� */
    SInt       connectState;
    struct qcConnectionNode * next;
}qcConnectionNode;

typedef struct qcConnectionList
{
    UInt               count;
    qcConnectionNode * connectionNode;
}qcConnectionList;

/* BUG-41307 User Lock ���� */
typedef struct qcUserLockNode
{
    SInt     id;
    iduMutex mutex;

    struct qcUserLockNode * next;
} qcUserLockNode;

typedef struct qcUserLockList
{
    UInt             count;
    qcUserLockNode * userLockNode;
} qcUserLockList;

typedef struct qcSessionObjInfo
{
    qcFileList       filelist;
    PDL_SOCKET       sendSocket;
    qcConnectionList connectionList;
    qcUserLockList   userLockList;
} qcSessionObjInfo;

/* PROJ-2657 UTL_SMTP ���� */
enum qcConnectionState
{
    QC_CONNECTION_STATE_NOCONNECT = 0,
    QC_CONNECTION_STATE_CONNECTED,
    QC_CONNECTION_STATE_SMTP_HELO,
    QC_CONNECTION_STATE_SMTP_MAIL,
    QC_CONNECTION_STATE_SMTP_RCPT,
    QC_CONNECTION_STATE_SMTP_OPEN,
    QC_CONNECTION_STATE_SMTP_DATA,
    QC_CONNECTION_STATE_SMTP_CRLF
};

/* PROJ-2657 UTL_SMTP ���� */
enum qcProtocolType
{
    QC_PROTOCOL_TYPE_NONE = 0,
    QC_PROTOCOL_TYPE_SMTP
};

// PROJ-2163 : �߰�
// Plan cache ���, �˻��� �ʿ��� bind param
// (���� ����� ���� Bitmap ���� ������ �� ����)
typedef struct qcPlanBindParam {
    UShort          id;
    UInt            type;
    SInt            precision;
    SInt            scale;
} qcPlanBindParam;

// PROJ-2163 : �߰�
// Plan cache ���, �˻��� �ʿ��� bind info
typedef struct qcPlanBindInfo
{
    qcPlanBindParam      * bindParam;
    UShort                 bindParamCount;
} qcPlanBindInfo;

// PROJ-1073 Package
typedef struct qcSessionPkgInfo
{
    smOID               pkgOID;
    smSCN               pkgSCN; /* PROJ-2268 */
    UInt                modifyCount;
    iduMemory         * pkgInfoMem;
    qcTemplate        * pkgTemplate;
    qcSessionPkgInfo  * next;
} qcSessionPkgInfo;

/* BUG-38294 */
typedef struct qcPRLQMemObj
{
    iduListNode  mNode;
    iduMemory  * mMemory;
} qcPRLQMemObj;

/* PROJ-2551 simple query ����ȭ
 *
 * [sql-da�� ȣ���ϴ� ���]
 * 1. ����� �� ������ ����Ǵ� ��� qcSimpleResult�� �������� �ʰ�
 *    ���� ������� ������ ���� �����Ѵ�.
 * 2. ����� �� ���� �ƴѰ�� execute�������� fetch���� ��� �����ϰ�
 *    qcSimpleResult chunk�� �����Ͽ� ��Ƴ��´�. ���� fetch��
 *    idx�� �����ذ��� �ٽ� client�� �����Ѵ�.
 *
 * [sql-da�� �ƴ����� fast execute�� ���]
 * 1. execute�������� fetch���� ��� �����ϰ�
 *    qcSimpleResult chunk�� �����Ͽ� ��Ƴ��´�. ���� fetch��
 *    idx�� �����ذ��� �ٽ� client�� �����Ѵ�.
 */
typedef struct qcSimpleResult
{
    SChar           * result;  // ������ �����ϴ� results buffer
    UInt              count;   // ������ ������ result count
    UInt              idx;     // Ŭ���̾�Ʈ���� fetch�� record index
    qcSimpleResult  * next;
} qcSimpleResult;

#endif /* _O_QC_H_ */